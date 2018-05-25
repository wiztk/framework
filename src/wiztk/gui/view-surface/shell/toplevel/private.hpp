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

#ifndef WIZTK_GUI_VIEW_SURFACE_SHELL_TOPLEVEL_PRIVATE_HPP_
#define WIZTK_GUI_VIEW_SURFACE_SHELL_TOPLEVEL_PRIVATE_HPP_

#include <wiztk/gui/view-surface.hpp>

#include "xdg-shell-unstable-v6-client-protocol.h"

namespace wiztk {
namespace gui {

struct ViewSurface::Shell::Toplevel::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private()
      : shell(nullptr), zxdg_toplevel(nullptr) {}

  ~Private() {
    if (zxdg_toplevel) zxdg_toplevel_v6_destroy(zxdg_toplevel);
  }

  Shell *shell;

  struct zxdg_toplevel_v6 *zxdg_toplevel;

  static void OnConfigure(void *data,
                          struct zxdg_toplevel_v6 *zxdg_toplevel_v6,
                          int32_t width,
                          int32_t height,
                          struct wl_array *states);

  static void OnClose(void *data,
                      struct zxdg_toplevel_v6 *zxdg_toplevel_v6);

  static const struct zxdg_toplevel_v6_listener kListener;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_VIEW_SURFACE_SHELL_TOPLEVEL_PRIVATE_HPP_
