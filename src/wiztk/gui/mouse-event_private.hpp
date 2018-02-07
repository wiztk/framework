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

#ifndef WIZTK_GUI_INTERNAL_MOUSE_EVENT_PRIVATE_HPP_
#define WIZTK_GUI_INTERNAL_MOUSE_EVENT_PRIVATE_HPP_

#include <wiztk/gui/mouse-event.hpp>

namespace wiztk {
namespace gui {

struct MouseEvent::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private()
      : surface(nullptr),
        serial(0),
        time(0),
        button(0),
        state(0),
        axis(0) {
  }

  ~Private() {}

  /** The surface this pointer hovers */
  ViewSurface *surface;

  uint32_t serial;

  base::Point2D surface_xy;

  uint32_t time;

  uint32_t button;
  uint32_t state;

  uint32_t axis;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_INTERNAL_MOUSE_EVENT_PRIVATE_HPP_
