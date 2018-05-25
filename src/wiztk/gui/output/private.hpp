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

#ifndef WIZTK_GUI_OUTPUT_PRIVATE_HPP_
#define WIZTK_GUI_OUTPUT_PRIVATE_HPP_

#include "wiztk/gui/output.hpp"

#include <wayland-client.h>

namespace wiztk {
namespace gui {

/**
 * @brief Private data in Output.
 */
struct Output::Private : public base::CountedDequeNodeBase {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);
  Private() = delete;

  explicit Private(Output *output)
      : base::CountedDequeNodeBase(),
        proprietor(output) {}

  ~Private() final {
    if (wl_output)
      wl_output_destroy(wl_output);
  }

  Output *proprietor = nullptr;

  struct wl_output *wl_output = nullptr;

  /** position within the global compositor space */
  base::Point2I position;

  /** physical_width width in millimeters of the output */
  base::SizeI physical_size;

  /** The size of a mode, given in physical hardware units of the output device */
  base::SizeI current_mode_size;
  base::SizeI preferred_mode_size;
  int32_t current_refresh_rate = 0;
  int32_t preferred_refresh_rate = 0;

  int subpixel = 0;  /**< enum value of wl_output_subpixel */
  int transform = WL_OUTPUT_TRANSFORM_NORMAL; /**< enum value of wl_output_transform */
  int scale = 1;

  /* vertical refresh rate in mHz */

  std::string make;
  std::string model;

  uint32_t id = 0;
  uint32_t version = 0;

  bool done = false;

  static void OnGeometry(void *data,
                         struct wl_output *wl_output,
                         int32_t x,
                         int32_t y,
                         int32_t physical_width,
                         int32_t physical_height,
                         int32_t subpixel,
                         const char *make,
                         const char *model,
                         int32_t transform);

  static void OnMode(void *data,
                     struct wl_output *wl_output,
                     uint32_t flags,
                     int32_t width,
                     int32_t height,
                     int32_t refresh);

  static void OnDone(void *data,
                     struct wl_output *wl_output);

  static void OnScale(void *data,
                      struct wl_output *wl_output,
                      int32_t factor);

  static const struct wl_output_listener kListener;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_OUTPUT_PRIVATE_HPP_
