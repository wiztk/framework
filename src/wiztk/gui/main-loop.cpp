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

#include "display_proxy.hpp"

#include <csignal>

#include <unistd.h>
#include <sys/signalfd.h>

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

namespace wiztk {
namespace gui {

MainLoop::SignalEvent::SignalEvent(MainLoop *main_loop)
    : main_loop_(main_loop) {
  sigset_t mask = {0};
  sigfillset(&mask);

  /* Block signals so that they aren't handled according to their default dispositions */
  if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
    handle_error("sigprocmask");

  signal_fd_ = signalfd(-1, &mask, SFD_CLOEXEC | SFD_NONBLOCK);
  _ASSERT(-1 != signal_fd_);
}

MainLoop::SignalEvent::~SignalEvent() {
  close(signal_fd_);
}

void MainLoop::SignalEvent::Run(uint32_t events) {
  _DEBUG("%s\n", __func__);

  struct signalfd_siginfo fdsi = {0};
  ssize_t s;

  s = read(signal_fd_, &fdsi, sizeof(struct signalfd_siginfo));
  if (s != sizeof(struct signalfd_siginfo))
    handle_error("read");

  if (fdsi.ssi_signo == SIGINT) {
    printf("Got SIGINT\n");
  } else if (fdsi.ssi_signo == SIGQUIT) {
    printf("Got SIGQUIT\n");
    exit(EXIT_SUCCESS);
  } else {
    printf("Read unexpected signal\n");
  }
}

// ----

MainLoop::WaylandEvent::WaylandEvent(MainLoop *main_loop)
    : main_loop_(main_loop) {

}

MainLoop::WaylandEvent::~WaylandEvent() = default;

void MainLoop::WaylandEvent::Run(uint32_t events) {
  if (events & EPOLLERR || events & EPOLLHUP) {
    main_loop_->Quit();
    return;
  }
  if (events & EPOLLIN) {
    if (wl_display_dispatch(main_loop_->wl_display_) == -1) {
      main_loop_->Quit();
      return;
    }
  }
  if (events & EPOLLOUT) {
    int ret = wl_display_flush(main_loop_->wl_display_);
    if (ret == 0) {
      main_loop_->ModifyWatchedFileDescriptor(wl_display_get_fd(main_loop_->wl_display_),
                                              this, EPOLLIN | EPOLLERR | EPOLLHUP);
    } else if (ret == -1 && errno != EAGAIN) {
      main_loop_->Quit();
      return;
    }
  }
}

// ----

MainLoop *MainLoop::Initialize(const Display *display) {
  MainLoop *main_loop = nullptr;
  try {
    main_loop = static_cast<MainLoop *>(Create([]() -> async::EventLoop * {
      return new MainLoop();
    }));
  } catch (const std::runtime_error &err) {
    throw err;
  }

  main_loop->WatchFileDescriptor(main_loop->signal_event_.signal_fd_,
                                 &main_loop->signal_event_,
                                 EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP);

  main_loop->wl_display_ = Display::Proxy::wl_display(display);
  _ASSERT(main_loop->wl_display_);

  main_loop->WatchFileDescriptor(wl_display_get_fd(main_loop->wl_display_),
                                 &main_loop->wayland_event_,
                                 EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP);

  return main_loop;
}

MainLoop::MainLoop()
    : signal_event_(this), wayland_event_(this) {}

MainLoop::~MainLoop() = default;

void MainLoop::Dispatch() {
  async::EventLoop::Dispatch();

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

  wl_display_dispatch_pending(wl_display_);
  int ret = wl_display_flush(wl_display_);
  if (ret < 0 && errno == EAGAIN) {
    Quit();
  }
}

} // namespace gui
} // namespace wiztk
