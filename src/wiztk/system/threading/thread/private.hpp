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

#ifndef WIZTK_SYSTEM_THREADING_THREAD_PRIVATE_HPP_
#define WIZTK_SYSTEM_THREADING_THREAD_PRIVATE_HPP_

#include "wiztk/system/threading/thread.hpp"
#include "wiztk/system/threading/thread-local.hpp"

namespace wiztk {
namespace system {
namespace threading {

/**
 * @brief Private structure in Thread.
 */
struct Thread::Private {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);

  Private() = default;

  Private(Delegate *d, const DelegateDeleter &deleter)
      : delegate(d), delegate_deleter(deleter) {}

  explicit Private(const Options &options)
      : options(options) {}

  ~Private() {
    if (nullptr != delegate) {
      delegate_deleter(delegate);
    }
  }

  ID id;

  Options options;

  Delegate *delegate = nullptr;

  DelegateDeleter delegate_deleter;

  State state = kNew;

  /**
   * @brief A static helper method to start a thread routine.
   * @param thread
   * @return
   */
  static void *StartRoutine(Thread *thread);

  static ID kMainThreadID;

};

/**
 * @brief Thread-specific data.
 */
struct Thread::Specific {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Specific);

  static Specific *GetCurrent();

  static Specific *Create();

  Specific() = default;

  ~Specific() = default;

  /**
   * @brief The Thread object.
   *
   * nullptr indicates the thread is detached.
   */
  Thread *thread = nullptr;

  static ThreadLocal<Specific> kPerThreadStorage;

};

} // namespace threading
} // namespace system
} // namespace wiztk

#endif // WIZTK_SYSTEM_THREADING_THREAD_PRIVATE_HPP_
