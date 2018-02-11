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

#ifndef WIZTK_SYSTEM_EVENT_EVENT_LOOP_HPP_
#define WIZTK_SYSTEM_EVENT_EVENT_LOOP_HPP_

#include "wiztk/system/threading/thread.hpp"

namespace wiztk {
namespace system {
namespace event {

// Forward declaration:
class EventLoop;

/**
 * @ingroup system
 * @brief An abstract factory class to create EventLoop instance.
 */
class AbstractEventLoopFactory {

 public:

  AbstractEventLoopFactory() = default;

  virtual ~AbstractEventLoopFactory() = default;

  virtual EventLoop *Create() = 0;

};

/**
 * @ingroup system
 * @brief Factory class creates default EventLoop.
 */
class DefaultEventLoopFactory : public AbstractEventLoopFactory {

 public:

  DefaultEventLoopFactory() = default;

  ~DefaultEventLoopFactory() final = default;

  EventLoop *Create() final;

};

/**
 * @ingroup system
 * @brief Message loop that waits for and dispatches messages (or events) in a thread.
 *
 * Each thread (created by threading::Thread) contains 0 or at most 1 message loop.
 *
 * Here's an example to implement a loop thread:
 *
 * @code
 * #include "wiztk/system/threading/thread.hpp"
 * #include "wiztk/system/message-loop.hpp"
 *
 * using namespace wiztk::system;
 *
 * class LoopThread : public threading::Thread {
 *
 *  public:
 *
 *    void Run() final {
 *      EventLoop.Prepare();
 *
 *      // ...
 *
 *      EventLoop.RunLoop();
 *    }
 *
 * };
 * @endcode
 */
class EventLoop {

  friend class DefaultEventLoopFactory;
  friend class threading::Thread;

 public:

  using Thread = threading::Thread;

  /**
   * @brief Get the message loop in current thread.
   * @return
   */
  static EventLoop *GetCurrent();

  /**
   * @brief Create and initialize a EventLoop object for the current thread.
   *
   * This static method will store the new EventLoop object in a TLS. You must
   * call this before actually starting the loop by RunLoop(). A EventLoop can be
   * ended by calling Quit().
   */
  static void Prepare(const AbstractEventLoopFactory &factory = DefaultEventLoopFactory());

  /**
   * @brief Run loop.
   */
  static void RunLoop();

  virtual ~EventLoop() = default;

  virtual void Quit();

 protected:

  EventLoop() = default;

  virtual void Loop();

 private:

  struct Private;

};

} // namespace event
} // namespace system
} // namespace wiztk

#endif // WIZTK_SYSTEM_EVENT_EVENT_LOOP_HPP_
