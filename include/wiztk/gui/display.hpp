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

#ifndef WIZTK_GUI_DISPLAY_HPP_
#define WIZTK_GUI_DISPLAY_HPP_

#include "wiztk/base/types.hpp"
#include "wiztk/base/sigcxx.hpp"
#include "wiztk/base/counted-deque.hpp"

#include "cursor.hpp"

#include <xkbcommon/xkbcommon.h>

#include <list>
#include <vector>
#include <set>
#include <memory>

namespace wiztk {
namespace gui {

// Forward declarations

class Output;
class Input;
class InputManager;
class OutputManager;

/**
 * @ingroup gui
 * @brief The global display which serves as the connection to the compositor.
 *
 * The display object can only be created and destroyed in Application. To get
 * the global display object, use the singleton application instance, for
 * example:
 *
 * @code
 * #include "wiztk/gui/application.hpp"
 *
 * // ...
 *
 * Display *display = Application::GetInstance()->GetDisplay();
 * @endcode
 */
class WIZTK_EXPORT Display {

  friend class Application;

 public:

  // Declarations:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Display);

  template<typename ... Args> using SignalRef = typename base::SignalRef<Args...>;
  template<typename ... Args> using Signal = typename base::Signal<Args...>;

  struct Global;
  struct Proxy;

 public:

  /**
   * @brief Get a deque of outputs
   * @return
   */
  OutputManager *GetOutputManager() const;

  /**
   * @brief Get the InputManager which contains a deque of input devices.
   * @return
   */
  InputManager *GetInputManager() const;

  /**
   * @brief Get a set of supported pixel formats
   * @return
   */
  const std::set<uint32_t> &GetPixelFormats() const;

  SignalRef<const Global &> unregister() { return unregister_; }

  /**
   * @brief Get a predefined cursor
   * @param cursor_type An enumeration of cursor type
   * @return
   */
  const Cursor *GetCursor(CursorType cursor_type) const;

 private:

  struct Private;

  Display();

  virtual ~Display();

  /**
   * @brief Connect to a wayland compositor
   * @param name The wayland compositor name
   *
   * This method is called once in Application.
   */
  void Connect(const char *name = nullptr);

  /**
   * @brief Disconnect from a wayland compositor
   *
   * This method will be called only when application exits.
   */
  void Disconnect() noexcept;

  void DestroyOutput(uint32_t id);

  void InitializeCursors();

  void ReleaseCursors();

  std::unique_ptr<Private> p_;

  Signal<const Global &> unregister_;

};

/**
  * @brief A simple structure to store information of wayland objects
  */
struct Display::Global {
  uint32_t id;
  std::string interface;
  uint32_t version;
};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_DISPLAY_HPP_
