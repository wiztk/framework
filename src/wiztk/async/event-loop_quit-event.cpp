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

#include "event-loop_quit-event.hpp"

#include <sys/eventfd.h>
#include <unistd.h>

namespace wiztk {
namespace async {

void EventLoop::QuitEvent::Trigger(EventLoop *event_loop) {
  auto *event = new QuitEvent(event_loop);  // Will be deleted when it's run.

  event_loop->WatchFileDescriptor(event->event_fd_, event, EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP);
  eventfd_write(event->event_fd_, 0);
}

EventLoop::QuitEvent::QuitEvent(EventLoop *event_loop)
    : event_loop_(event_loop) {
  // make sure to use non-block mode:
  event_fd_ = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
}

EventLoop::QuitEvent::~QuitEvent() {
  close(event_fd_);
}

void EventLoop::QuitEvent::Run(uint32_t events) {
  if (events & EPOLLOUT) {
    uint64_t buf;
    eventfd_read(event_fd_, &buf);
  }

  event_loop_->UnwatchFileDescriptor(event_fd_);
  event_loop_->running_ = false;

  delete this;
}

} // namespace async
} // namespace wiztk
