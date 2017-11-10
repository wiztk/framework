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

#include "internal/mouse-event_private.hpp"

#include <wiztk/gui/surface.hpp>

namespace wiztk {
namespace gui {

using base::PointI;
using base::PointD;

MouseEvent::MouseEvent(Input *input)
    : InputEvent(input) {
  p_.reset(new Private);
}

MouseEvent::~MouseEvent() {

}

Surface *MouseEvent::GetSurface() const {
  return p_->surface;
}

uint32_t MouseEvent::GetSerial() const {
  return p_->serial;
}

const PointD &MouseEvent::GetSurfaceXY() const {
  return p_->surface_xy;
}

PointD MouseEvent::GetWindowXY() const {
  PointD xy;

  PointI pos = p_->surface->GetWindowPosition();

  xy.x = pos.x + p_->surface_xy.x;
  xy.y = pos.y + p_->surface_xy.y;

  return xy;
}

uint32_t MouseEvent::GetButton() const {
  return p_->button;
}

uint32_t MouseEvent::GetState() const {
  return p_->state;
}

uint32_t MouseEvent::GetAxis() const {
  return p_->axis;
}

} // namespace gui
} // namespace wiztk
