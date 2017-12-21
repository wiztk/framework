/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
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

#include <wiztk/gui/application.hpp>

#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

#include <csignal>
#include <iostream>

#include <wiztk/gui/theme.hpp>
#include <wiztk/gui/abstract-view.hpp>
#include <wiztk/gui/view-surface.hpp>

#include "internal/display_private.hpp"

using std::cerr;
using std::endl;

namespace wiztk {
namespace gui {

class Application::EpollTask : public AbstractEpollTask {

 public:

  explicit EpollTask(Application *app)
      : AbstractEpollTask(), app_(app) {}

  ~EpollTask() final = default;

  void Run(uint32_t events) override;

 private:

  Application *app_;

};

/**
 * @ingroup gui_intern
 * @brief The private structure used in Application
 */
struct Application::Private {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);

  explicit Private(Application *app)
      : running(true), epoll_fd(-1), epoll_task(app), argc(0), argv(nullptr) {}

  ~Private() = default;

  bool running;
  int epoll_fd;

  EpollTask epoll_task;

  int argc;
  char **argv;

  std::thread::id thread_id;

  base::Deque<TaskNode> task_deque;

  /**
* @brief Create an epoll file descriptor
* @return a nonnegative file descriptor or -1
*/
  static int CreateEpollFd();

  /**
   * @brief Set close-on-exec or close the epoll file descriptor
   * @param fd The epoll file descriptor created in CreateEpollFd()
   * @return a nonnegative file descriptor or -1
   */
  static int SetCloexecOrClose(int fd);

  /**
   * @brief Set close-on-exec flag of the given file descripor
   * @param fd The epoll file descriptor passed from CreateEpollFd()
   * @return
   *     0 - success
   *    -1 - fail
   */
  static int SetCloexec(int fd);

  static void HandleSignalInt(int);

  static const int kMaxEpollEvents = 16;

};

int Application::Private::CreateEpollFd() {
  int fd = 0;

#ifdef EPOLL_CLOEXEC
  fd = epoll_create1(EPOLL_CLOEXEC);
  if (fd >= 0)
    return fd;
  if (errno != EINVAL)
    return -1;
#endif

  fd = epoll_create(1);
  return SetCloexecOrClose(fd);
}

int Application::Private::SetCloexecOrClose(int fd) {
  if (SetCloexec(fd) != 0) {
    close(fd);
    return -1;
  }
  return fd;
}

int Application::Private::SetCloexec(int fd) {
  long flags;

  if (fd == -1)
    return -1;

  flags = fcntl(fd, F_GETFD);
  if (flags == -1)
    return -1;

  if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1)
    return -1;

  return 0;
}

void Application::Private::HandleSignalInt(int) {
  _DEBUG("%s\n", "Get SIGINT");
  kInstance->Exit();
}

// ----------

void Application::EpollTask::Run(uint32_t events) {
  Display::kDisplay->p_->epoll_events = events;
  if (events & EPOLLERR || events & EPOLLHUP) {
    Application::kInstance->Exit();
    return;
  }
  if (events & EPOLLIN) {
    if (wl_display_dispatch(Display::kDisplay->p_->wl_display) == -1) {
      Application::kInstance->Exit();
      return;
    }
  }
  if (events & EPOLLOUT) {
    struct epoll_event ep;
    int ret = wl_display_flush(Display::kDisplay->p_->wl_display);
    if (ret == 0) {
      ep.events = EPOLLIN | EPOLLERR | EPOLLHUP;
      ep.data.ptr = &app_->p_->epoll_task;
      epoll_ctl(app_->p_->epoll_fd, EPOLL_CTL_MOD,
                Display::kDisplay->p_->fd, &ep);
    } else if (ret == -1 && errno != EAGAIN) {
      Application::kInstance->Exit();
      return;
    }
  }
}

Application *Application::kInstance = nullptr;

Application::Application(int argc, char *argv[]) {
  p_ = std::make_unique<Private>(this);

  p_->argc = argc;
  p_->argv = argv;
  p_->thread_id = std::this_thread::get_id();

  if (kInstance != nullptr)
    throw std::runtime_error("Error! There should be only one application instance!");

  kInstance = this;

  // Set log handler to a lambda function
  wl_log_set_handler_client([](const char *format, va_list args) {
    vfprintf(stderr, format, args);
  });

  Display::kDisplay = new Display;

  try {
    Display::kDisplay->Connect(NULL);
  } catch (const std::runtime_error &e) {
    cerr << e.what() << endl;
    exit(EXIT_FAILURE);
  }

  // Load theme
  Theme::Initialize();

  p_->epoll_fd = Private::CreateEpollFd();
  WatchFileDescriptor(Display::kDisplay->p_->fd, EPOLLIN | EPOLLERR | EPOLLHUP, &p_->epoll_task);
}

Application::~Application() {
  close(p_->epoll_fd);
  Display::kDisplay->Disconnect();

  Theme::Release();
  delete Display::kDisplay;
  Display::kDisplay = nullptr;

  kInstance = nullptr;
}

int Application::Run() {
  struct sigaction sigint;
  sigint.sa_handler = Private::HandleSignalInt;
  sigemptyset(&sigint.sa_mask);
  sigint.sa_flags = SA_RESETHAND;
  sigaction(SIGINT, &sigint, NULL);

  struct epoll_event ep[Private::kMaxEpollEvents];
  int count = 0;
  int ret = 0;
  TaskNode *task = nullptr;
  base::Deque<TaskNode>::Iterator task_deque_iterator;
  base::Deque<ViewSurface::RenderTask>::Iterator render_task_iterator;
  base::Deque<ViewSurface::CommitTask>::Iterator commit_task_iterator;

  while (true) {

    /*
     * Run idle tasks (process geometries)
     */
    task_deque_iterator = p_->task_deque.begin();
    while (task_deque_iterator != p_->task_deque.end()) {
      task = task_deque_iterator.get();
      task_deque_iterator.remove();
      task->Run();
      task_deque_iterator = p_->task_deque.begin();
    }

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

    wl_display_dispatch_pending(Display::kDisplay->p_->wl_display);

    if (!p_->running) break;

    ret = wl_display_flush(Display::kDisplay->p_->wl_display);
    if (ret < 0 && errno == EAGAIN) {
      _DEBUG("%s\n", "Error when flush display");
      ep[0].events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP;
      ep[0].data.ptr = &p_->epoll_task;
      epoll_ctl(p_->epoll_fd, EPOLL_CTL_MOD,
                Display::kDisplay->p_->fd, &ep[0]);
    } else if (ret < 0) {
      break;
    }

    AbstractEpollTask *epoll_task = nullptr;
    count = epoll_wait(p_->epoll_fd, ep, Private::kMaxEpollEvents, -1);
    for (int i = 0; i < count; i++) {
      epoll_task = static_cast<AbstractEpollTask *>(ep[i].data.ptr);
      if (epoll_task) epoll_task->Run(ep[i].events);
    }
  }

  return 0;
}

void Application::Exit() {
  p_->running = false;

  // TODO: check if need to clean other resources
}

void Application::WatchFileDescriptor(int fd, uint32_t events, AbstractEpollTask *epoll_task) {
  struct epoll_event ep;
  ep.events = events;
  ep.data.ptr = epoll_task;
  epoll_ctl(p_->epoll_fd, EPOLL_CTL_ADD, fd, &ep);
}

void Application::UnwatchFileDescriptor(int fd) {
  epoll_ctl(p_->epoll_fd, EPOLL_CTL_DEL, fd, NULL);
}

int Application::GetArgC() {
  return p_->argc;
}

char **Application::GetArgV() const {
  return p_->argv;
}

const std::thread::id &Application::GetThreadID() {
  return p_->thread_id;
}

base::Deque<TaskNode> &Application::GetTaskDeque() {
  return p_->task_deque;
}

} // namespace gui
} // namespace wiztk
