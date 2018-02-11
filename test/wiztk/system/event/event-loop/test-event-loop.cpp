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

#include "wiztk/system/async/event-loop.hpp"

#include <iostream>

using namespace wiztk;
using namespace wiztk::system;

class MyLoopThread : public threading::Thread {

 public:

  MyLoopThread() = default;
  ~MyLoopThread() final = default;

 protected:

  void Run() final {
    async::EventLoop::Prepare();
    std::cout << __func__ << std::endl;
    async::EventLoop::RunLoop();
  }

};

TEST_F(TestEventLoop, prepare_1) {
  MyLoopThread my_thread;
  my_thread.Start();
  my_thread.Join();

  ASSERT_TRUE(true);
}