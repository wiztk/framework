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

#include "display/proxy.hpp"

#include "wiztk/gui/application.hpp"

namespace wiztk {
namespace gui {

Cursor *Cursor::Create(struct wl_cursor *wl_cursor) {
  Display *display = Application::GetInstance()->GetDisplay();

  Cursor *cursor = new Cursor;

  cursor->wl_surface_ = wl_compositor_create_surface(Display::Proxy::wl_compositor(display));
  cursor->wl_cursor_ = wl_cursor;

  struct wl_cursor_image *image = wl_cursor->images[0];
  struct wl_buffer *buffer = wl_cursor_image_get_buffer(image);

  // TODO: attach buffer to surface
  wl_surface_attach(cursor->wl_surface_, buffer, 0, 0);

  return cursor;
}

Cursor::~Cursor() {
}

} // namespace gui
} // namespace wiztk
