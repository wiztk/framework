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
 * @brief Input manager controlled by Display.
 */
class InputManager {

 public:

  InputManager() = default;

  ~InputManager() = default;

  void Insert(Input *input, int index) {
    deque_.insert(input, index);
  }

  void Clear() {
    deque_.clear([](base::BinodeBase *obj) { delete obj; });
  }

 private:

  base::CountedDeque<Input> deque_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_INPUT_MANAGER_HPP_
