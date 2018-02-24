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

#include <atomic>

using namespace wiztk;
using namespace wiztk::system;
using namespace wiztk::async;

static EventLoop *loop = nullptr;

class MyLoopThread : public threading::Thread {

 public:

  MyLoopThread() = default;
  ~MyLoopThread() final = default;

 protected:

  void Run() final {
    EventLoop *event_loop = EventLoop::Create();
    std::cout << __func__ << std::endl;
    loop = event_loop;
    event_loop->Run();
    loop = nullptr;
  }

};

/**
 * @brief Test EventLoop::Quit() called in another thread.
 */
TEST_F(TestEventLoop, quit_1) {
  MyLoopThread my_thread;
  my_thread.Start();

  sleep(2);
  loop->Quit();

  my_thread.Join();

  ASSERT_TRUE(true);
}
