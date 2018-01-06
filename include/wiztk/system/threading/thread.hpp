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

#include <pthread.h>

namespace wiztk {
namespace system {
namespace threading {

/**
 * @ingroup system_threading
 * @brief A class to create and control a thread.
 */
class Thread {

 public:

  /**
   * @brief Thread ID.
   */
  class ID {
    friend class Thread;
    friend bool operator==(const ID &id1, const ID &id2);
    pthread_t pthread_ = 0;
  };

  class Attribute;

 public:

  Thread() = default;

  virtual ~Thread() = default;

  void Start();

  void Join();

  const ID &id() const { return id_; }

 protected:

  virtual void Run() {/* Override in sub class */}

 private:

  struct Private;

  ID id_;

};

class Thread::Attribute {
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

  Attribute() {
    pthread_attr_init(&pthread_attribute_);
  }

  ~Attribute() {
    pthread_attr_destroy(&pthread_attribute_);
  }

  void SetDetachState(DetachStateType state_type) {
    pthread_attr_setdetachstate(&pthread_attribute_, state_type);
  }

  DetachStateType GetDetachState() const {
    int val = 0;
    pthread_attr_getdetachstate(&pthread_attribute_, &val);
    return static_cast<DetachStateType>(val);
  }

  void SetScope(ScopeType scope_type) {
    pthread_attr_setscope(&pthread_attribute_, scope_type);
  }

  ScopeType GetScope() const {
    int val = 0;
    pthread_attr_getscope(&pthread_attribute_, &val);
    return static_cast<ScopeType>(val);
  }

  void SetStackSize(size_t stack_size) {
    pthread_attr_setstacksize(&pthread_attribute_, stack_size);
  }

  size_t GetStackSize() const {
    size_t stack_size = 0;
    pthread_attr_getstacksize(&pthread_attribute_, &stack_size);
    return stack_size;
  }

 private:

  pthread_attr_t pthread_attribute_;

};

/**
 * @brief Compare thread IDs.
 * @param id1
 * @param id2
 * @return
 */
bool operator==(const Thread::ID &id1, const Thread::ID &id2);

bool operator==(const Thread &thread1, const Thread &thread2);

} // namespace threading
} // namespace system
} // namespace wiztk

#endif // WIZTK_BASE_THREADING_THREAD_HPP_
