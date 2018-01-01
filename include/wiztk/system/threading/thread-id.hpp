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

#ifndef WIZTK_SYSTEM_THREADING_THREAD_ID_HPP_
#define WIZTK_SYSTEM_THREADING_THREAD_ID_HPP_

#include <pthread.h>

namespace wiztk {
namespace system {
namespace threading {

/**
 * @ingroup system_threading
 * @brief Thread ID.
 */
class ThreadID {

  friend class Thread;

 public:

  ThreadID() = default;

  ~ThreadID() = default;

 private:

  pthread_t pthread_id_ = 0;

};

} // namespace threading
} // namespace system
} // namespace wiztk

#endif // WIZTK_SYSTEM_THREADING_THREAD_ID_HPP_
