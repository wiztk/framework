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

#include "display/private.hpp"

#include "wiztk/gui/region.hpp"
#include "wiztk/gui/application.hpp"

namespace wiztk {
namespace gui {

Region::Region()
    : wl_region_(nullptr) {
  Display *display = Application::GetInstance()->GetDisplay();
  wl_region_ = wl_compositor_create_region(Display::Private::Get(*display).wl_compositor);
}

Region::~Region() {
  if (wl_region_)
    wl_region_destroy(wl_region_);
}

} // namespace gui
} // namespace wiztk
