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

#ifndef WIZTK_GUI_INTERNAL_SURFACE_SHELL_PRIVATE_HPP
#define WIZTK_GUI_INTERNAL_SURFACE_SHELL_PRIVATE_HPP

#include <wiztk/gui/surface.hpp>

#include "xdg-shell-unstable-v6-client-protocol.h"

namespace wiztk {
namespace gui {

struct Surface::Shell::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private()
      : zxdg_surface(nullptr) {}

  ~Private() {
    if (zxdg_surface) zxdg_surface_v6_destroy(zxdg_surface);
  }

  struct zxdg_surface_v6 *zxdg_surface;

  static void OnConfigure(void *data,
                          struct zxdg_surface_v6 *zxdg_surface_v6,
                          uint32_t serial);

  static const struct zxdg_surface_v6_listener kListener;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_INTERNAL_SURFACE_SHELL_PRIVATE_HPP
