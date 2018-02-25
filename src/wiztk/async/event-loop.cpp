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

#include "event-loop_private.hpp"

#include "wiztk/async/message.hpp"
#include "wiztk/async/message-queue.hpp"
#include "wiztk/async/scheduler.hpp"
#include "event-loop_quit-event.hpp"

#include <unistd.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>

namespace wiztk {
namespace async {

EventLoop::FactoryType EventLoop::kDefaultFactory = []() -> EventLoop * {
  return new EventLoop();
};

EventLoop::EventLoop() {
  epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
  _ASSERT(-1 != epoll_fd_);
}

EventLoop::~EventLoop() {
  if (-1 != epoll_fd_)
    close(epoll_fd_);
}

EventLoop *EventLoop::GetCurrent() {
  return Private::kPerThreadStorage.Get();
}

EventLoop *EventLoop::Create(const FactoryType &factory) {
  if (nullptr != Private::kPerThreadStorage.Get())
    throw std::runtime_error("Error! Only one EventLoop can be created per thread!");

  EventLoop *event_loop = factory();
  Private::kPerThreadStorage.Set(event_loop);
  return event_loop;
}

void EventLoop::Run() {
  struct epoll_event events[max_events_];

  running_ = true;

  DispatchMessage();
  if (!running_) return;

  int count = epoll_wait(epoll_fd_, events, max_events_, timeout_);
  while (true) {
    if (count > 0) {
      for (int i = 0; i < count; ++i) {
        auto *event = static_cast<AbstractEvent *> (events[i].data.ptr);
        if (nullptr != event)
          event->Run(events->events);
      }
    }

    // TODO: process event queue.
    DispatchMessage();

    if (!running_) break;
    count = epoll_wait(epoll_fd_, events, max_events_, timeout_);
  }
}

void EventLoop::Quit() {
  QuitEvent::Trigger(this);
}

bool EventLoop::WatchFileDescriptor(int fd, AbstractEvent *event, uint32_t events) {
  _ASSERT(nullptr != event);
  struct epoll_event ev = {0};
  ev.events = events;
  ev.data.ptr = event;

  return (0 == epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev));
}

bool EventLoop::ModifyWatchedFileDescriptor(int fd, AbstractEvent *event, uint32_t events) {
  _ASSERT(event);
  struct epoll_event ev = {0};
  ev.events = events;
  ev.data.ptr = event;

  return (0 == epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &ev));
}

bool EventLoop::UnwatchFileDescriptor(int fd) {
  return (0 == epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr));
}

Scheduler EventLoop::GetScheduler() {
  return Scheduler(this);
}

void EventLoop::DispatchMessage() {
  Message *msg = message_queue_.PopFront();
  while (nullptr != msg) {
    msg->Execute();
    msg = message_queue_.PopFront();
  }
}

} // namespace async
} // namespace wiztk
