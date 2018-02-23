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

#include "test-event-loop.hpp"

#include "wiztk/async/event-loop.hpp"
#include "wiztk/system/threading/thread.hpp"

#include <iostream>

#include <sys/eventfd.h>
#include <sys/epoll.h>

using namespace wiztk;
using namespace wiztk::system;
using namespace wiztk::async;

static int event_fd = -1;
static uint64_t buf = 0;

class MyEventLoop : public EventLoop {

 public:

  MyEventLoop();

  ~MyEventLoop() final;

 protected:

  void Dispatch() final;

 private:

  int epoll_fd_ = -1;

};

MyEventLoop::MyEventLoop() {
  epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
  _ASSERT(epoll_fd_ >= 0);
}

MyEventLoop::~MyEventLoop() {
  if (epoll_fd_ >= 0)
    close(epoll_fd_);
}

void MyEventLoop::Dispatch() {
  struct epoll_event ev = {0};
  ev.events = EPOLLIN | EPOLLOUT;
  ev.data.fd = event_fd;

  epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, event_fd, &ev);

  int count = 0;

  struct epoll_event events[16];
  count = epoll_wait(epoll_fd_, events, 16, 0);
  while (true) {
    if (count > 0) {
      std::cout << "Get epoll count: " << count << std::endl;

      for (int i = 0; i < count; i++) {
        eventfd_read(event_fd, &buf);
        std::cout << "Read event value: " << buf << std::endl;
      }
    }

    if (buf >= 9) break;

    count = epoll_wait(epoll_fd_, events, 16, 0);
  }
}

// ----

class MyLoopThread : public threading::Thread {

 public:

  MyLoopThread() = default;
  ~MyLoopThread() final = default;

 protected:

  void Run() final {
    EventLoop *event_loop = EventLoop::Create();
    std::cout << __func__ << std::endl;
    event_loop->Run();
  }

};

TEST_F(TestEventLoop, prepare_1) {
//  event_fd = eventfd(0, EFD_CLOEXEC);

  MyLoopThread my_thread;
  my_thread.Start();

//  for (int i = 0; i < 10; i++) {
//    eventfd_write(event_fd, i);
//    if (buf >= 9) break;
//    sleep(1);
//  }

  sleep(5);

  my_thread.Join();

//  close(event_fd);

  ASSERT_TRUE(true);
}
