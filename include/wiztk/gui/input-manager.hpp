/*
 * Copyright 2017 The WizTK Authors.
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

#ifndef WIZTK_GUI_INPUT_MANAGER_HPP_
#define WIZTK_GUI_INPUT_MANAGER_HPP_

#include "wiztk/base/counted-deque.hpp"

#include "wiztk/gui/input.hpp"

namespace wiztk {
namespace gui {

/**
 * @ingroup gui
 * @brief Singleton input manager controlled by Display.
 *
 * The InputManager is created when connecting a client display and destroyed
 * when disconnecting. To get the singleton instance, use the method in Display,
 * for example:
 *
 * @code
 * #include "wiztk/gui/application.hpp"
 * #include "wiztk/gui/display.hpp"
 *
 * // ...
 *
 * Display* display = Application::GetInstance()->GetDisplay();
 * InputManager *input_mgr = display->GetInputManager();
 * @endcode
 */
class WIZTK_EXPORT InputManager {

  friend class Display;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(InputManager);

 public:

 private:

  typedef base::CountedDeque<Input::Private> InputPrivateDeque;

  InputManager() = default;

  ~InputManager();

  void AddInput(Input *input);

  void Clear();

  InputPrivateDeque deque_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_INPUT_MANAGER_HPP_
