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

#include "private.hpp"
#include "surface/private.hpp"
#include "abstract-shell-view/private.hpp"

#include "wiztk/base/bit.hpp"

namespace wiztk {
namespace gui {

using base::Bit;

const struct zxdg_toplevel_v6_listener Surface::Shell::Toplevel::Private::kListener = {
    OnConfigure,
    OnClose
};

void Surface::Shell::Toplevel::Private::OnConfigure(void *data,
                                                    struct zxdg_toplevel_v6 * /* zxdg_toplevel_v6 */,
                                                    int32_t width,
                                                    int32_t height,
                                                    struct wl_array *states) {
  Toplevel *_this = static_cast<Toplevel *>(data);

  void *p = nullptr;
  int value = 0;

  wl_array_for_each(p, states) {
    uint32_t state = *((uint32_t *) p);
    switch (state) {
      case ZXDG_TOPLEVEL_V6_STATE_MAXIMIZED: {
        Bit::Set<int>(value, kStateMaskMaximized);
        break;
      }
      case ZXDG_TOPLEVEL_V6_STATE_FULLSCREEN: {
        Bit::Set<int>(value, kStateMaskFullscreen);
        break;
      }
      case ZXDG_TOPLEVEL_V6_STATE_RESIZING: {
        Bit::Set<int>(value, kStateMaskResizing);
        break;
      }
      case ZXDG_TOPLEVEL_V6_STATE_ACTIVATED: {
        Bit::Set<int>(value, kStateMaskActivated);
        break;
      }
      default:
        /* Unknown state */
        break;
    }
  }

  auto *shell_view = dynamic_cast<AbstractShellView *>(_this->p_->shell->surface_->p_->event_handler);
  if (shell_view)
    shell_view->p_->OnXdgToplevelConfigure(width, height, value);
}

void Surface::Shell::Toplevel::Private::OnClose(void *data, struct zxdg_toplevel_v6 * /* zxdg_toplevel_v6 */) {
  auto *_this = static_cast<Toplevel *>(data);
  auto *shell_view = dynamic_cast<AbstractShellView *>(_this->p_->shell->surface_->p_->event_handler);
  if (shell_view)
    shell_view->p_->OnXdgToplevelClose();
}

} // namespace gui
} // namespace wiztk
