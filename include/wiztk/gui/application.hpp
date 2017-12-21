/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
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

#ifndef WIZTK_GUI_APPLICATION_HPP_
#define WIZTK_GUI_APPLICATION_HPP_

#include <cstdint>
#include <thread>

#include "wiztk/base/deque.hpp"

#include "task-node.hpp"
#include "display.hpp"

namespace wiztk {
namespace gui {

class AbstractEpollTask;

/**
 * @ingroup gui
 * @brief Single object manages an application's main event loop and global resources
 */
class WIZTK_EXPORT Application {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Application);
  Application() = delete;

  /**
   * @brief Construct a single application instance
   * @param argc The argc parameter passed from main()
   * @param argv The argv parameter passed from main()
   *
   * @warning An application instance should be constructed only once in the main function.
   */
  Application(int argc, char *argv[]);

  /**
   * @brief Destructor
   */
  ~Application();

  /**
   * @brief Run the main event loop
   * @return
   *
   * Internally this object uses epoll to implement the main event loop.
   */
  int Run();

  /**
   * @brief Exit the main event loop
   */
  void Exit();

  /**
   * @brief Watch a given file descriptor in the main epoll event loop
   * @param fd
   * @param events Epoll events flags
   * @param epoll_task An AbstractEpollTask object, the virtual Run() method will be called
   *                   when there's event arrived on the fd
   */
  void WatchFileDescriptor(int fd, uint32_t events, AbstractEpollTask *epoll_task);

  /**
   * @brief Unwatch the given file descriptor
   * @param fd
   */
  void UnwatchFileDescriptor(int fd);

  /**
   * @brief Get the argc parameter
   * @return
   */
  int GetArgC();

  /**
   * @brief Get the argv parameter
   * @return
   */
  char **GetArgV() const;

  /**
   * @brief Get the main thread id
   * @return A const reference to the main thread id
   */
  const std::thread::id &GetThreadID();

  /**
   * @brief Get the defferred task deque
   * @return
   */
  base::Deque<TaskNode> &GetTaskDeque();

  static inline Application *instance() { return kInstance; }

 private:

  class EpollTask;
  struct Private;

  std::unique_ptr<Private> p_;

  static Application *kInstance;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_APPLICATION_HPP_
