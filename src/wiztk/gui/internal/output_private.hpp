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

#ifndef WIZTK_GUI_INTERNAL_OUTPUT_PRIVATE_HPP_
#define WIZTK_GUI_INTERNAL_OUTPUT_PRIVATE_HPP_

#include <wiztk/gui/output.hpp>

namespace wiztk {
namespace gui {

struct Output::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private()
      : wl_output(nullptr),
        current_refresh_rate(0),
        preferred_refresh_rate(0),
        subpixel(0),
        transform(WL_OUTPUT_TRANSFORM_NORMAL),
        scale(1),
        id(0),
        version(0) {}

  ~Private() {
    if (wl_output)
      wl_output_destroy(wl_output);
  }

  struct wl_output *wl_output;

  /** position within the global compositor space */
  base::Point2I position_;

  /** physical_width width in millimeters of the output */
  base::SizeI physical_size_;

  /** The size of a mode, given in physical hardware units of the output device */
  base::SizeI current_mode_size_;
  base::SizeI preferred_mode_size_;
  int32_t current_refresh_rate;
  int32_t preferred_refresh_rate;

  int subpixel;  /**< enum value of wl_output_subpixel */
  int transform; /**< enum value of wl_output_transform */
  int scale;

  /* vertical refresh rate in mHz */

  std::string make_;
  std::string model_;

  uint32_t id;
  uint32_t version;

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

#endif // WIZTK_GUI_INTERNAL_OUTPUT_PRIVATE_HPP_
