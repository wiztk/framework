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

#include "keymap.hpp"

#include "display/private.hpp"

#include "wiztk/gui/application.hpp"

namespace wiztk {
namespace gui {

Keymap::~Keymap() {
  if (xkb_keymap_)
    xkb_keymap_unref(xkb_keymap_);
}

void Keymap::Setup(const char *string, enum xkb_keymap_format format, enum xkb_keymap_compile_flags flags) {
  Destroy();

  Display *display = Application::GetInstance()->GetDisplay();
  xkb_keymap_ = xkb_keymap_new_from_string(Display::Private::Get(*display).xkb_context, string, format, flags);
  if (nullptr == xkb_keymap_)
    throw std::runtime_error("FATAL! Cannot create XKB keymap!");
}

void Keymap::Destroy() {
  if (xkb_keymap_) {
    xkb_keymap_unref(xkb_keymap_);
    xkb_keymap_ = nullptr;
  }
}

} // namespace gui
} // namespace wiztk
