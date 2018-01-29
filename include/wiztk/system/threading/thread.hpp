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

#include "wiztk/base/abstract-runnable.hpp"

#include <pthread.h>
#include <functional>

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
   * @brief Alias to base::AbstractRunnable.
   */
  using AbstractRunnable = base::AbstractRunnable;

  /**
   * @brief Thread ID.
   */
  class ID {

    friend class Thread;
    friend bool operator==(const ID &id1, const ID &id2);

   public:

    ID() = delete;
    ID(const ID &) = delete;
    ID &operator=(const ID &) = delete;

    ~ID() = default;

   private:

    explicit ID(Thread *thread)
        : thread_(thread) {}

    pthread_t native_ = 0;
    Thread *thread_ = nullptr;

  };

  class Attribute;

  typedef std::function<void(AbstractRunnable *)> DeleterType;

  /**
   * @brief A default function object which will delete runnable in destructor.
   */
  static const DeleterType kDefaultDeleter;

 public:

  /**
   * @brief Default constructor.
   */
  Thread();

  explicit Thread(AbstractRunnable *runnable,
                  const DeleterType &deleter = kDefaultDeleter);

  virtual ~Thread();

  void Start();

  void Join();

  const ID &id() const { return id_; }

 protected:

  virtual void Run() {/* Override in sub class */}

 private:

  struct Private;

  ID id_;

  AbstractRunnable *runnable_ = nullptr;

  DeleterType deleter_;

};

/**
 * @brief Thread attribute.
 */
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

  Attribute();

  ~Attribute();

  void SetDetachState(DetachStateType state_type);

  DetachStateType GetDetachState() const;

  void SetScope(ScopeType scope_type);

  ScopeType GetScope() const;

  void SetStackSize(size_t stack_size);

  size_t GetStackSize() const;

 private:

  pthread_attr_t native_;

};

/**
 * @brief Compare thread IDs.
 * @param id1
 * @param id2
 * @return
 */
bool operator==(const Thread::ID &id1, const Thread::ID &id2);

} // namespace threading
} // namespace system
} // namespace wiztk

#endif // WIZTK_BASE_THREADING_THREAD_HPP_
