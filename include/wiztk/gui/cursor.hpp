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

#ifndef WIZTK_GUI_CURSOR_HPP_
#define WIZTK_GUI_CURSOR_HPP_

#include <wayland-cursor.h>
#include <wayland-client.h>

namespace wiztk {
namespace gui {

class Cursor {

  friend class Input;

  Cursor(const Cursor &) = delete;
  Cursor &operator=(const Cursor &) = delete;

 public:

  static Cursor *Create(struct wl_cursor *wl_cursor);

  ~Cursor();

  void Commit() const {
    wl_surface_commit(wl_surface_);
  }

  int32_t hotspot_x() const {
    return wl_cursor_->images[wl_cursor_->image_count - 1]->hotspot_x;
  }

  int32_t hotspot_y() const {
    return wl_cursor_->images[wl_cursor_->image_count - 1]->hotspot_y;
  }

 private:

  Cursor() : wl_surface_(nullptr), wl_cursor_(nullptr) {}

  struct wl_surface* wl_surface_;

  struct wl_cursor *wl_cursor_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_CURSOR_HPP_
