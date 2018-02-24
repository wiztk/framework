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

#ifndef WIZTK_ASYNC_EVENT_LOOP_HPP_
#define WIZTK_ASYNC_EVENT_LOOP_HPP_

#include "wiztk/base/macros.hpp"

#include "wiztk/async/type.hpp"
#include "wiztk/async/message-queue.hpp"
#include "wiztk/async/scheduler.hpp"

#include <functional>

namespace wiztk {
namespace async {

/**
 * @ingroup async
 * @brief An abstract epoll event.
 */
class WIZTK_EXPORT AbstractEvent {

  friend class EventLoop;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(AbstractEvent);

  AbstractEvent() = default;

  explicit AbstractEvent(uint32_t events)
      : events_(events) {}

  virtual ~AbstractEvent() = default;

 protected:

  virtual void Run(uint32_t events) = 0;

 private:

  uint32_t events_ = kRead | kWrite | kError;

};

/**
 * @ingroup async
 * @brief Event loop that waits for and dispatches events in a thread.
 *
 * Each thread (created by threading::Thread) contains 0 or at most 1 event loop.
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
 *      EventLoop* event_loop = EventLoop::Create();
 *
 *      // ...
 *
 *      event_loop->Run();
 *    }
 *
 * };
 * @endcode
 *
 * Use the Quit() in the run loop.
 */
class WIZTK_EXPORT EventLoop {

  friend class Scheduler;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(EventLoop);

  typedef std::function<EventLoop *()> FactoryType;

  static FactoryType kDefaultFactory;

  /**
   * @brief Get the message loop in current thread.
   * @return
   */
  static EventLoop *GetCurrent();

  /**
   * @brief Create and initialize a EventLoop object for the current thread.
   * @param factory A factory functor which create a new EventLoop object.
   * @return
   *
   * This static method will store the new EventLoop object in a TLS. You must
   * call this before actually starting the loop by RunLoop(). A EventLoop can be
   * ended by calling Quit().
   */
  static EventLoop *Create(const FactoryType &factory = kDefaultFactory);

  virtual ~EventLoop();

  /**
   * @brief Run loop.
   */
  void Run();

  /**
   * @brief Push back a quit event and clean up.
   */
  void Quit();

  /**
   * @brief Watch a given file descriptor in the main event loop
   * @param fd An integer file descriptor.
   */
  bool WatchFileDescriptor(int fd, AbstractEvent *event);

  /**
   * @brief Unwatch the given file descriptor.
   * @param fd An integer file descriptor.
   */
  bool UnwatchFileDescriptor(int fd);

  /**
   * @brief Get an Scheduler for this EventLoop.
   * @return
   */
  Scheduler GetScheduler();

 protected:

  EventLoop();

  virtual void Dispatch();

 private:

  struct Private;

  class QuitEvent;

  int epoll_fd_ = -1;

  int max_events_ = 16;

  int timeout_ = 0;

  bool running_ = false;

  MessageQueue message_queue_;

};

} // namespace async
} // namespace wiztk

#endif // WIZTK_ASYNC_EVENT_LOOP_HPP_
