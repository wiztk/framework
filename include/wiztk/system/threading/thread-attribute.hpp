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

#ifndef WIZTK_SYSTEM_THREADING_THREAD_ATTRIBUTE_HPP_
#define WIZTK_SYSTEM_THREADING_THREAD_ATTRIBUTE_HPP_

#include <pthread.h>

namespace wiztk {
namespace system {
namespace threading {

/**
 * @ingroup system_threading
 * @brief Thread attribute.
 */
class ThreadAttribute {

  friend class Thread;

 public:

  enum DetachStateType {
    kDetachStateCreateDetached = PTHREAD_CREATE_DETACHED,
    kDetachStateCreateJoinable = PTHREAD_CREATE_JOINABLE
  };

  enum ScopeType {
    kScopeSystem = PTHREAD_SCOPE_SYSTEM,
    kScopeProcess = PTHREAD_SCOPE_PROCESS
  };

  enum SchedulerType {
    kSchedulerInherit = PTHREAD_INHERIT_SCHED,
    kSchedulerExplicit = PTHREAD_EXPLICIT_SCHED
  };

  ThreadAttribute();

  ~ThreadAttribute();

  void SetDetachState(DetachStateType state_type);

  DetachStateType GetDetachState() const;

  void SetScope(ScopeType scope_type);

  ScopeType GetScope() const;

  void SetStackSize(size_t stack_size);

  size_t GetStackSize() const;

 private:

  pthread_attr_t pthread_attribute_;

};

} // namespace threading
} // namespace system
} // namespace wiztk

#endif // WIZTK_SYSTEM_THREADING_THREAD_ATTRIBUTE_HPP_
