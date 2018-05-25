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

#include "signal-event.hpp"

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

  main_loop_->WatchFileDescriptor(signal_fd_,
                                  this,
                                  EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP);
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

} // namespace gui
} // namespace wiztk
