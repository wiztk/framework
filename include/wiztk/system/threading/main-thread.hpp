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

#ifndef WIZTK_SYSTEM_THREADING_MAIN_THREAD_HPP_
#define WIZTK_SYSTEM_THREADING_MAIN_THREAD_HPP_

#include "wiztk/system/threading/thread.hpp"

namespace wiztk {
namespace system {
namespace threading {

/**
 * @ingroup system_threading
 * @brief A singleton class represents the main thread.
 *
 * In your program there should be only one MainThread object at the same time and it
 * does not use the Run() method. It registers itself as the main thread so that you
 * can use the Thread::GetCurrent() to access this object. You should use a sub class
 * for your customized feature and it's recommended to create one object at the
 * beginning in your program.
 *
 * In GUI application this is usually the UI thread.
 */
class MainThread : public Thread {

 public:

  /**
   * @brief Declare this object is non-copyable.
   */
  WIZTK_DECLARE_NONCOPYABLE(MainThread);

 public:

  /**
   * @brief Default constructor.
   *
   * The default constructor will register itself so that you can use the
   * Thread::GetCurrent() to access this object.
   *
   * If you try to create 2 or more MainThread objects, it will raise
   * an exception.
   */
  MainThread();

  /**
   * @brief Default move constructor.
   */
  MainThread(MainThread &&) = default;

  /**
   * @brief Destructor.
   *
   * The destructor will unregister it self from the global main thread object.
   */
  ~MainThread() override;

  /**
   * @brief Default move operator.
   * @return
   */
  MainThread &operator=(MainThread &&) = default;

 protected:

  /**
   * @brief Final virtual method which does nothing.
   */
  void Run() final;

};

} // namespace threading
} // namespace system
} // namespace wiztk

#endif // WIZTK_SYSTEM_THREADING_MAIN_THREAD_HPP_
