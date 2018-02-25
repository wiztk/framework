/*
 * Copyright 2017 The WizTK Authors.
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

#include "display_private.hpp"

#include "wiztk/base/property.hpp"

#include "wiztk/gui/application.hpp"
#include "wiztk/gui/main-loop.hpp"
#include "wiztk/gui/theme.hpp"

#include <unistd.h>
#include <fcntl.h>

#include <csignal>
#include <iostream>

using std::cerr;
using std::endl;

namespace wiztk {
namespace gui {

/**
 * @brief The private structure used in Application
 */
struct Application::Private {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);

  Private() = default;

  ~Private() = default;

  Display *display = nullptr;

  int argc = 0;

  char **argv = nullptr;

  Thread::ID thread_id;

  MainLoop *main_loop = nullptr;

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

Application *Application::GetInstance() {
  return kInstance;
}

Application *Application::kInstance = nullptr;

Application::Application(int argc, char *argv[]) {
  if (kInstance != nullptr)
    throw std::runtime_error("Error! There should be only one application instance!");

  p_ = std::make_unique<Private>();
  p_->argc = argc;
  p_->argv = argv;

  kInstance = this;

  // Set log handler to a lambda function
  wl_log_set_handler_client([](const char *format, va_list args) {
    vfprintf(stderr, format, args);
  });

  __PROPERTY__(display) = new Display;

  try {
    __PROPERTY__(display)->Connect(NULL);
  } catch (const std::runtime_error &e) {
    cerr << e.what() << endl;
    exit(EXIT_FAILURE);
  }

  // Load theme
  Theme::Initialize();

  __PROPERTY__(main_loop) = MainLoop::Initialize(__PROPERTY__(display));
}

Application::~Application() {
  __PROPERTY__(display)->Disconnect();
  delete __PROPERTY__(display);
  __PROPERTY__(display) = nullptr;

  Theme::Release();

  kInstance = nullptr;
}

int Application::Run() {
  struct sigaction sigint;
  sigint.sa_handler = Private::HandleSignalInt;
  sigemptyset(&sigint.sa_mask);
  sigint.sa_flags = SA_RESETHAND;
  sigaction(SIGINT, &sigint, NULL);

//  __PROPERTY__(main_loop)->Prepare();
  __PROPERTY__(main_loop)->Run();

  return 0;
}

void Application::Exit() {
  __PROPERTY__(main_loop)->Quit();
  // TODO: check if need to clean other resources
}

int Application::GetArgC() {
  return p_->argc;
}

char **Application::GetArgV() const {
  return p_->argv;
}

Display *Application::GetDisplay() const {
  return __PROPERTY__(display);
}

const Application::Thread::ID &Application::GetThreadID() {
  return p_->thread_id;
}

} // namespace gui
} // namespace wiztk
