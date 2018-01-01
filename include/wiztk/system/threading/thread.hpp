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

/**
 * @file Header file for Thread class.
 */

#ifndef WIZTK_SYSTEM_THREADING_THREAD_HPP_
#define WIZTK_SYSTEM_THREADING_THREAD_HPP_

#include "wiztk/system/threading/thread-id.hpp"
#include "wiztk/system/threading/thread-attribute.hpp"

namespace wiztk {
namespace system {
namespace threading {

/**
 * @ingroup system_threading
 * @brief A class to create and control a thread.
 */
class Thread {

 public:

  Thread() = default;

  virtual ~Thread() = default;

  void Start();

  void Join();

  const ThreadID &id() const { return id_; }

 protected:

  virtual void Run() {/* Override in sub class */}

 private:

  struct Private;

  ThreadID id_;

};

} // namespace threading
} // namespace system
} // namespace wiztk

#endif // WIZTK_BASE_THREADING_THREAD_HPP_
