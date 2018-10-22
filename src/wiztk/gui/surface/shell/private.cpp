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

namespace wiztk {
namespace gui {

const struct zxdg_surface_v6_listener Surface::Shell::Private::kListener = {
    OnConfigure
};

void Surface::Shell::Private::OnConfigure(void *data,
                                          struct zxdg_surface_v6 *zxdg_surface_v6,
                                          uint32_t serial) {
  auto *_this = static_cast<Shell *>(data);
  auto *shell_view = dynamic_cast<AbstractShellView *>(_this->surface_->p_->event_handler);
  if (shell_view)
    shell_view->p_->OnXdgSurfaceConfigure(serial);
}

} // namespace gui
} // namespace wiztk
