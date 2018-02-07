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

#include <stdexcept>
#include "keyboard_state.hpp"
#include "keymap.hpp"

namespace wiztk {
namespace gui {

KeyboardState::~KeyboardState() {
  if (xkb_state_) xkb_state_unref(xkb_state_);
}

void KeyboardState::Setup(const Keymap &keymap) {
  Destroy();

  xkb_state_ = xkb_state_new(keymap.xkb_keymap_);
  if (nullptr == xkb_state_) {
    xkb_keymap_unref(keymap.xkb_keymap_);
    throw std::runtime_error("FATAL! Cannot create keyboard state!");
  }
}

void KeyboardState::Destroy() {
  if (xkb_state_) {
    xkb_state_unref(xkb_state_);
    xkb_state_ = nullptr;
  }
}

} // namespace gui
} // namespace wiztk
