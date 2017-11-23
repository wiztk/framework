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
#include "wiztk/base/compound-deque.hpp"

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

/**
 * @ingroup gui
 * @brief The global display which serves as the connection to the compositor
 */
class WIZTK_EXPORT Display {

  friend class Application;
  friend class Output;
  friend class Input;
  friend class Callback;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Display);

  using CompoundDeque = base::CompoundDeque;
  template<typename ... ParamTypes> using SignalRefT = typename base::SignalRefT<ParamTypes...>;
  template<typename ... ParamTypes> using SignalT = typename base::SignalT<ParamTypes...>;

  struct Global;
  struct Proxy;

  /**
   * @brief Get a deque of outputs
   * @return
   */
  static const CompoundDeque &GetOutputs();

  /**
   * @brief Get a deque of inputs
   * @return
   */
  static const CompoundDeque &GetInputs();

  /**
   * @brief Get a set of supported pixel formats
   * @return
   */
  static const std::set<uint32_t> &GetPixelFormats();

  SignalRefT<const Global &> unregister() { return unregister_; }

  /**
   * @brief Get a predefined cursor
   * @param cursor_type An enumeration of cursor type
   * @return
   */
  static const Cursor *GetCursor(CursorType cursor_type);

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

  void AddOutput(Output *output, int index = 0);

  void DestroyOutput(uint32_t id);

  void AddInput(Input *input, int index = 0);

  void InitializeCursors();

  void ReleaseCursors();

  std::unique_ptr<Private> p_;

  SignalT<const Global &> unregister_;

  static Display *kDisplay;

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
