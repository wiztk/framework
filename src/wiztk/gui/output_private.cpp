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

#include "output_private.hpp"

namespace wiztk {
namespace gui {

const struct wl_output_listener Output::Private::kListener = {
    OnGeometry,
    OnMode,
    OnDone,
    OnScale
};

void Output::Private::OnGeometry(void *data,
                                 struct wl_output * /* wl_output */,
                                 int32_t x,
                                 int32_t y,
                                 int32_t physical_width,
                                 int32_t physical_height,
                                 int32_t subpixel,
                                 const char *make,
                                 const char *model,
                                 int32_t transform) {
  auto *_this = static_cast<Output *>(data);

  _this->p_->position.x = x;
  _this->p_->position.y = y;
  _this->p_->physical_size.width = physical_width;
  _this->p_->physical_size.height = physical_height;
  _this->p_->subpixel = subpixel;
  _this->p_->make = make;
  _this->p_->model = model;
  _this->p_->transform = transform;
}

void Output::Private::OnMode(void *data,
                             struct wl_output * /* wl_output */,
                             uint32_t flags,
                             int32_t width,
                             int32_t height,
                             int32_t refresh) {
  auto *_this = static_cast<Output *>(data);

  if (flags & WL_OUTPUT_MODE_CURRENT) {
    _this->p_->current_mode_size.width = width;
    _this->p_->current_mode_size.height = height;
    _this->p_->current_refresh_rate = refresh;
  } else if (flags & WL_OUTPUT_MODE_PREFERRED) {
    _this->p_->preferred_mode_size.width = width;
    _this->p_->preferred_mode_size.height = height;
    _this->p_->preferred_refresh_rate = refresh;
  }
}

void Output::Private::OnDone(void *data, struct wl_output * /* wl_output */) {
  auto *_this = static_cast<Output *>(data);
  _this->p_->done = true;
}

void Output::Private::OnScale(void *data, struct wl_output * /* wl_output */, int32_t factor) {
  auto *_this = static_cast<Output *>(data);
  _this->p_->scale = factor;
}

} // namespace gui
} // namespace wiztk
