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

#include <stdexcept>
#include "wiztk/system/threading/thread.hpp"

namespace wiztk {
namespace system {
namespace threading {

/**
 * @ingroup system_threading_intern
 * @brief Private structure in Thread.
 */
struct Thread::Private {

  /**
   * @brief A static helper method to start a thread routine.
   * @param thread
   * @return
   */
  static void *StartRoutine(Thread *thread);

};

void *Thread::Private::StartRoutine(Thread *thread) {
  thread->Run();
  return nullptr;
}

// -------

void Thread::Start() {
  typedef void *(*fn)(void *);
  int ret = pthread_create(&id_.pthread_id_,
                           nullptr,
                           reinterpret_cast<fn>(Thread::Private::StartRoutine),
                           this);
  if (ret != 0) throw std::runtime_error("Error! Fail to start a thread!");
}

void Thread::Join() {
  int ret = pthread_join(id_.pthread_id_, nullptr);
  if (ret != 0) throw std::runtime_error("Error! Fail to join a thread!");
}

}  // namespace threading
}  // namespace system
}  // namespace wiztk
