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

#include "wiztk/base/macros.hpp"

#include "wiztk/system/threading/thread-local.hpp"

#include <pthread.h>

#include <memory>
#include <functional>

namespace wiztk {
namespace system {

// Forward declaration
class EventLoop;

namespace threading {

/**
 * @ingroup system_threading
 * @brief A class to create and control a thread.
 */
class WIZTK_EXPORT Thread {

  friend class MainThread;

 public:

  WIZTK_DECLARE_NONCOPYABLE(Thread);

  /**d
   * @brief A nested class represents a thread ID.
   */
  class ID;
  class Delegate;
  class Attribute;

  /**
   * @brief A structure to be used when constructing a Thread object only.
   */
  struct Option {

    Option() = default;
    ~Option() = default;

    bool use_event_loop = true;

  };

  typedef std::function<void(Delegate *)> DelegateDeleterType;

  /**
   * @brief A default function object which will delete delegate in destructor.
   */
  static const DelegateDeleterType kDefaultDelegateDeleter;

 public:

  /**
   * @brief Default constructor.
   */
  Thread();

  explicit Thread(Delegate *delegate,
                  const DelegateDeleterType &deleter = kDefaultDelegateDeleter);

  explicit Thread(const Option &option);

  Thread(Thread &&other) noexcept;

  virtual ~Thread();

  Thread &operator=(Thread &&other) noexcept;

  /**
   * @brief Start this thread.
   */
  void Start();

  /**
   * @brief Join with a terminated thread.
   */
  void Join();

  void Detach();

  /**
   * @brief Get the thread ID.
   * @return
   */
  const ID &GetID() const;

  /**
   * @brief Get the current thread object.
   * @return
   *
   * @note In the main thread, this will always return the main thread object.
   */
  static Thread *GetCurrent();

 protected:

  virtual void Run() {/* Override in sub class */}

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

/**
 * @brief Thread delegate.
 */
class Thread::Delegate {
  friend class Thread;

 public:

  Delegate() = default;
  Delegate(const Delegate &) = default;
  Delegate &operator=(const Delegate &) = default;
  Delegate(Delegate &&) = default;
  Delegate &operator=(Delegate &&) = default;
  virtual ~Delegate() = default;

 protected:

  virtual void Run() {/* Override in subclass */}

};

/**
 * @brief Thread ID
 */
class Thread::ID {

  friend class Thread;
  friend bool operator==(const Thread::ID &id1, const Thread::ID &id2);
  friend bool operator!=(const Thread::ID &id1, const Thread::ID &id2);

 public:

  WIZTK_DECLARE_NONCOPYABLE(ID);

  ID() {
    native_ = pthread_self();
  }

  ID(ID &&) = default;
  ~ID() = default;
  ID &operator=(ID &&) = default;

  /**
   * @brief Get the current thread ID.
   * @return
   */
  static ID GetCurrent();

 private:

  pthread_t native_ = 0;

};

/**
 * @brief Compare thread IDs.
 * @param id1
 * @param id2
 * @return
 */
bool operator==(const Thread::ID &id1, const Thread::ID &id2);

bool operator!=(const Thread::ID &id1, const Thread::ID &id2);

} // namespace threading
} // namespace system
} // namespace wiztk

#endif // WIZTK_BASE_THREADING_THREAD_HPP_
