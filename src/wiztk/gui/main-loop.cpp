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

#include "wiztk/base/property.hpp"

#include "wiztk/gui/main-loop.hpp"
#include "wiztk/gui/view-surface.hpp"

#include "main-loop_private.hpp"

#include "display_proxy.hpp"

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

  main_loop->WatchFileDescriptor(main_loop->__PROPERTY__(signal_event_).signal_fd_,
                                 &main_loop->__PROPERTY__(signal_event_),
                                 EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP);

  main_loop->__PROPERTY__(wl_display_) = Display::Proxy::wl_display(display);
  _ASSERT(main_loop->__PROPERTY__(wl_display_));

  main_loop->WatchFileDescriptor(wl_display_get_fd(main_loop->__PROPERTY__(wl_display_)),
                                 &main_loop->__PROPERTY__(wayland_event_),
                                 EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP);

  return main_loop;
}

MainLoop::MainLoop() {
  p_ = std::make_unique<_Private>(this);
}

MainLoop::~MainLoop() = default;

void MainLoop::DispatchMessage() {
  using namespace base;
  using namespace async;

  EventLoop::DispatchMessage();

  QueuedTask *task = nullptr;
  Deque<ViewSurface::RenderTask>::Iterator render_it;
  Deque<ViewSurface::CommitTask>::Iterator commit_it;

  /*
   * Draw contents on every surface requested
   */
  render_it = ViewSurface::kRenderTaskDeque.begin();
  while (render_it != ViewSurface::kRenderTaskDeque.end()) {
    task = render_it.get();
    render_it.remove();
    task->Run();
    render_it = ViewSurface::kRenderTaskDeque.begin();
  }

  /*
   * Commit every surface requested
   */
  commit_it = ViewSurface::kCommitTaskDeque.begin();
  while (commit_it != ViewSurface::kCommitTaskDeque.end()) {
    task = commit_it.get();
    commit_it.remove();
    task->Run();
    commit_it = ViewSurface::kCommitTaskDeque.begin();
  }

  wl_display_dispatch_pending(__PROPERTY__(wl_display_));
  int ret = wl_display_flush(__PROPERTY__(wl_display_));
  if (ret < 0 && errno == EAGAIN) {
    Quit();
  }
}

} // namespace gui
} // namespace wiztk
