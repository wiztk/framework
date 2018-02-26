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

#ifndef WIZTK_GUI_APPLICATION_HPP_
#define WIZTK_GUI_APPLICATION_HPP_

#include "wiztk/base/macros.hpp"

#include <memory>

namespace wiztk {
namespace gui {

// Forward declaration:
class Display;

/**
 * @ingroup gui
 * @brief Single object manages an application's main event loop and global resources
 */
class WIZTK_EXPORT Application {

 public:

  /**
   * @brief Declare this class is non-copyable and non-movable.
   */
  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Application);

  /**
   * @brief Disable the default constructor.
   */
  Application() = delete;

 public:

  /**
   * @brief Get the instance of the singleton Application object.
   * @return Pointer to the Application object.
   */
  static Application *GetInstance();

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
   * @brief Get the argc parameter
   * @return
   */
  int GetArgc() const;

  /**
   * @brief Get the argv parameter
   * @return
   */
  char **GetArgv() const;

  /**
   * @brief Get the the wayland client display.
   * @return
   */
  Display *GetDisplay() const;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

  static Application *kInstance;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_APPLICATION_HPP_
