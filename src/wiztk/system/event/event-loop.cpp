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

#include "../threading/thread_private.hpp"

#include <unistd.h>
#include <iostream>

namespace wiztk {
namespace system {
namespace event {

EventLoop *DefaultEventLoopFactory::Create() {
  return new EventLoop();
}

// -----

EventLoop *EventLoop::GetCurrent() {
  return Private::kPerThreadStorage.Get();
}

void EventLoop::Prepare(const AbstractEventLoopFactory &factory) {
  if (nullptr != Private::kPerThreadStorage.Get())
    throw std::runtime_error("Error! Only one MessageLoop can be created per thread!");

  Private::kPerThreadStorage.Set(const_cast<AbstractEventLoopFactory &>(factory).Create());
  // Note: the MessageLoop object will be deleted when the thread stops.
}

void EventLoop::RunLoop() {
  EventLoop *message_loop = GetCurrent();
  if (nullptr == message_loop)
    throw std::runtime_error("Error! No MessageLoop was created (Prepare()) for this thread!");

  message_loop->Loop();
}

void EventLoop::Quit() {
  // TODO: stop the loop
}

void EventLoop::Loop() {
  // Get message queue
  // dispatch messages
  for (;;) {
    std::cout << __func__ << std::endl;
    sleep(1);  // For test.
  }
}

}
}
}
