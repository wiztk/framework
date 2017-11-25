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

#ifndef WIZTK_GUI_KEY_EVENT_HPP_
#define WIZTK_GUI_KEY_EVENT_HPP_

#include "input-event.hpp"

#include <linux/input-event-codes.h>

namespace wiztk {
namespace gui {

class ViewSurface;

enum Key {
  kKey_ESC = KEY_ESC,
  kKey_1 = KEY_1,
  kKey_2 = KEY_2
  // TODO: add more key code
};

class KeyEvent : public InputEvent {

  friend class Input;

  KeyEvent() = delete;
  KeyEvent(const KeyEvent &) = delete;
  KeyEvent &operator=(const KeyEvent &) = delete;

 public:

  KeyEvent(Input *input)
      : InputEvent(input),
        serial_(0),
        time_(0),
        key_(0),
        state_(0),
        mods_depressed_(0),
        mods_latched_(0),
        mods_locked_(0),
        group_(0),
        surface_(nullptr) {
  }

  uint32_t serial() const {
    return serial_;
  }

  uint32_t time() const {
    return time_;
  }

  uint32_t key() const {
    return key_;
  }

  uint32_t state() const {
    return state_;
  }

  uint32_t mods_depressed() const {
    return mods_depressed_;
  }

 private:

  ~KeyEvent() {}

  uint32_t serial_;
  uint32_t time_;
  uint32_t key_;
  uint32_t state_;

  uint32_t mods_depressed_;
  uint32_t mods_latched_;
  uint32_t mods_locked_;
  uint32_t group_;

  ViewSurface *surface_;
};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_KEY_EVENT_HPP_
