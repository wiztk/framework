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

#include "thread_private.hpp"

#include "../async/event-loop_private.hpp"

namespace wiztk {
namespace system {
namespace threading {

Thread::ID Thread::Private::kMainThreadID;

void *Thread::Private::StartRoutine(Thread *thread) {
  thread->p_->state = kRunning;
  auto *specific = Specific::Create();
  specific->thread = thread;

  Specific::kPerThreadStorage.Set(specific);

  if (thread->p_->delegate) thread->p_->delegate->Run();
  else thread->Run();

  auto *obj = Specific::kPerThreadStorage.Get();
  Specific::kPerThreadStorage.Set(nullptr);
  delete obj;

  async::EventLoop *message_loop = async::EventLoop::Private::kPerThreadStorage.Get();
  if (nullptr != message_loop) {
    async::EventLoop::Private::kPerThreadStorage.Set(nullptr);
    delete message_loop;
  }

  thread->p_->state = kTerminated;
  pthread_exit((void *) thread);
}

// -----

ThreadLocal<Thread::Specific> Thread::Specific::kPerThreadStorage;

Thread::Specific *Thread::Specific::GetCurrent() {
  return kPerThreadStorage.Get();
}

Thread::Specific *Thread::Specific::Create() {
  if (nullptr != kPerThreadStorage.Get())
    throw std::runtime_error("Error! Only on Specific can be created per thread!");

  auto *object = new Specific();
  kPerThreadStorage.Set(object);
  return object;
}

}
}
}
