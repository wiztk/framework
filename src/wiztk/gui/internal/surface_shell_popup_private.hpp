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

#ifndef WIZTK_GUI_INTERNAL_SURFACE_SHELL_POPUP_PRIVATE_HPP_
#define WIZTK_GUI_INTERNAL_SURFACE_SHELL_POPUP_PRIVATE_HPP_

#include "xdg-shell-unstable-v6-client-protocol.h"

namespace wiztk {
namespace gui {

struct Surface::Shell::Popup::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private()
      : shell(nullptr),
        zxdg_popup(nullptr),
        zxdg_positioner(nullptr) {}

  ~Private() {
    if (zxdg_positioner) {
      zxdg_positioner_v6_destroy(zxdg_positioner);
    }
    if (zxdg_popup) {
      zxdg_popup_v6_destroy(zxdg_popup);
    }
  }

  Shell *shell;

  struct zxdg_popup_v6 *zxdg_popup;
  struct zxdg_positioner_v6 *zxdg_positioner;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_INTERNAL_SURFACE_SHELL_POPUP_PRIVATE_HPP_
