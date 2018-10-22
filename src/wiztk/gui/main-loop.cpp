/*
 * Copyright 2017 - 2018 The WizTK Authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "main-loop/private.hpp"
#include "display/private.hpp"

#include "wiztk/base/property.hpp"

#include "wiztk/gui/surface.hpp"

namespace wiztk {
namespace gui {

MainLoop *MainLoop::Initialize(const Display *display) {
  MainLoop *main_loop = nullptr;
  try {
    main_loop = static_cast<MainLoop *>(Create([]() -> async::EventLoop * {
      return new MainLoop();
    }));
  } catch (const std::runtime_error &err) {
    throw err;
  }

  main_loop->__PROPERTY__(wl_display) = Display::Private::Get(*display).wl_display;
  _ASSERT(main_loop->__PROPERTY__(wl_display));

  main_loop->WatchFileDescriptor(main_loop->__PROPERTY__(signal_event).signal_fd_,
                                 &main_loop->__PROPERTY__(signal_event),
                                 EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP);

  main_loop->WatchFileDescriptor(wl_display_get_fd(main_loop->__PROPERTY__(wl_display)),
                                 &main_loop->__PROPERTY__(display_event),
                                 EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP);

  return main_loop;
}

MainLoop::MainLoop() {
  p_ = std::make_unique<Private>(this);
}

MainLoop::~MainLoop() = default;

void MainLoop::DispatchMessage() {
  using base::Deque;
  using async::EventLoop;

  EventLoop::DispatchMessage();

  QueuedTask *task = nullptr;
  Deque<Surface::RenderTask>::Iterator render_it;
  Deque<Surface::CommitTask>::Iterator commit_it;

  /*
   * Draw contents on every surface requested
   */
  render_it = Surface::kRenderTaskDeque.begin();
  while (render_it != Surface::kRenderTaskDeque.end()) {
    task = render_it.get();
    render_it.remove();
    task->Run();
    render_it = Surface::kRenderTaskDeque.begin();
  }

  /*
   * Commit every surface requested
   */
  commit_it = Surface::kCommitTaskDeque.begin();
  while (commit_it != Surface::kCommitTaskDeque.end()) {
    task = commit_it.get();
    commit_it.remove();
    task->Run();
    commit_it = Surface::kCommitTaskDeque.begin();
  }

  wl_display_dispatch_pending(__PROPERTY__(wl_display));
  int ret = wl_display_flush(__PROPERTY__(wl_display));
  if (ret < 0 && errno == EAGAIN) {
    Quit();
  }
}

} // namespace gui
} // namespace wiztk
