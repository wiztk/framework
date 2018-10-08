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
#include "wiztk/base/abstract-runnable.hpp"

#include <pthread.h>

#include <memory>
#include <functional>

namespace wiztk {
namespace system {

namespace threading {

/**
 * @ingroup system_threading
 * @brief A class to create and control a thread.
 */
class WIZTK_EXPORT Thread {

  friend class MainThread;

 public:

  WIZTK_DECLARE_NONCOPYABLE(Thread);

  /**
   * @brief A nested class represents a thread ID.
   */
  class ID;

  /**
   * @brief Enumeration indicates the state of a Thread.
   */
  enum State {
    kNew, /**< A thread that has not started */
    kRunning, /**< The thread has been started */
    kTerminated /**< A thread that has exited */
  };

  /**
   * @brief A nested class represents a runnable delegate run in thread.
   */
  class Delegate;

  /**
   * @brief A structure to be used when constructing a Thread object only.
   */
  struct WIZTK_EXPORT Options {

    /**
     * @brief The stack size for the thread to use.
     *
     * Use 0 for the default maximum.
     */
    size_t stack_size = 0;

    /**
     * @brief If the thread is created joinable.
     */
    bool joinable = true;

  };

  /**
   * @brief A function object to delete Delegate object in destructor.
   */
  typedef std::function<void(Delegate *)> DelegateDeleter;

  /**
   * @brief A default function object which will delete delegate in destructor.
   */
  static const DelegateDeleter kDefaultDelegateDeleter;

  /**
   * @brief Alternate function object which will not clean up Delegate.
   */
  static const DelegateDeleter kLeakyDelegateDeleter;

 public:

  /**
   * @brief Default constructor.
   */
  Thread();

  /**
   * @brief Constructor with a Delegate object.
   * @param delegate
   * @param deleter
   */
  explicit Thread(Delegate *delegate,
                  const DelegateDeleter &deleter = kDefaultDelegateDeleter);

  /**
   * @brief Constructor with options.
   * @param options
   */
  explicit Thread(const Options &options);

  /**
   * @brief Move constructor.
   * @param other
   */
  Thread(Thread &&other) noexcept;

  /**
   * @brief Destructor.
   */
  virtual ~Thread();

  /**
   * @brief Move assignment.
   * @param other
   * @return
   */
  Thread &operator=(Thread &&other) noexcept;

  /**
   * @brief Start this thread with the default options.
   */
  void Start();

  void Start(const Options &options);

  void Stop();

  /**
   * @brief Join with a terminated thread.
   */
  void Join();

  void Detach();

  void Yield();

  /**
   * @brief Returns if the threading is started and still running.
   * @return
   */
  bool IsRunning() const;

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

  /**
   * @brief A virtual method for subclass to run the routine in this thread.
   *
   * If this thread was constructed using a separate Delegate object, then that
   * Delegate object's Run() method is called; otherwise, this method does
   * nothing and returns.
   *
   * Subclasses of Thread should override this method.
   */
  virtual void Run();

 private:

  class Attribute;
  struct Specific;
  struct Private;

  std::unique_ptr<Private> p_;

};

/**
 * @brief Thread delegate.
 */
class WIZTK_EXPORT Thread::Delegate : public base::AbstractRunnable<> {
  friend class Thread;

 public:

  /** @brief Default constructor. */
  Delegate() = default;

  /** @brief Default destructor. */
  ~Delegate() override = default;

 protected:

  /** @brief Virtual function to be overrided in subclass */
  void Run() override;

};

/**
 * @brief Thread ID
 */
class WIZTK_EXPORT Thread::ID {

  friend class Thread;
  friend bool operator==(const Thread::ID &id1, const Thread::ID &id2);
  friend bool operator!=(const Thread::ID &id1, const Thread::ID &id2);

 public:

  WIZTK_DECLARE_NONCOPYABLE(ID);

  /** @brief Default constructor. */
  ID() {
    native_ = pthread_self();
  }

  /** @brief Default move constructor. */
  ID(ID &&) = default;

  /** @brief Default destructor. */
  ~ID() = default;

  /** @brief Default move assignment. */
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
