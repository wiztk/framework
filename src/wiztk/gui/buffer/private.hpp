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

#ifndef WIZTK_GUI_BUFFER_PRIVATE_HPP_
#define WIZTK_GUI_BUFFER_PRIVATE_HPP_

#include "wiztk/gui/buffer.hpp"

#include <wayland-client.h>

namespace wiztk {
namespace gui {

struct Buffer::Private {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);

  Private() = default;

  ~Private() = default;

  struct wl_buffer *wl_buffer = nullptr;

  /**
   * @brief Position on surface
   */
  Point position;

  Size size;

  int32_t stride = 0;

  uint32_t format = 0;

  int offset = 0;

  void *data = nullptr;

  static void OnRelease(void *data, struct wl_buffer *buffer);

  static const struct wl_buffer_listener kListener;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_BUFFER_PRIVATE_HPP_
