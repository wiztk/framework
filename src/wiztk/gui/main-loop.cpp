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
  async::EventLoop::DispatchMessage();

  TaskNode *task = nullptr;
  base::Deque<ViewSurface::RenderTask>::Iterator render_task_iterator;
  base::Deque<ViewSurface::CommitTask>::Iterator commit_task_iterator;

  /*
   * Draw contents on every surface requested
   */
  render_task_iterator = ViewSurface::kRenderTaskDeque.begin();
  while (render_task_iterator != ViewSurface::kRenderTaskDeque.end()) {
    task = render_task_iterator.get();
    render_task_iterator.remove();
    task->Run();
    render_task_iterator = ViewSurface::kRenderTaskDeque.begin();
  }

  /*
   * Commit every surface requested
   */
  commit_task_iterator = ViewSurface::kCommitTaskDeque.begin();
  while (commit_task_iterator != ViewSurface::kCommitTaskDeque.end()) {
    task = commit_task_iterator.get();
    commit_task_iterator.remove();
    task->Run();
    commit_task_iterator = ViewSurface::kCommitTaskDeque.begin();
  }

  wl_display_dispatch_pending(__PROPERTY__(wl_display_));
  int ret = wl_display_flush(__PROPERTY__(wl_display_));
  if (ret < 0 && errno == EAGAIN) {
    Quit();
  }
}

} // namespace gui
} // namespace wiztk
