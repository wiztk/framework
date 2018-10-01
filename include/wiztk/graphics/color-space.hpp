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

#ifndef WIZTK_GRAPHICS_COLOR_SPACE_HPP_
#define WIZTK_GRAPHICS_COLOR_SPACE_HPP_

#include "wiztk/base/macros.hpp"

#include <memory>

namespace wiztk {
namespace graphics {

/**
 * @ingroup graphics
 * @brief Color space
 */
class WIZTK_EXPORT ColorSpace {

  friend class ImageInfo;

  friend bool operator==(const ColorSpace &, const ColorSpace &);
  friend bool operator!=(const ColorSpace &, const ColorSpace &);

 public:

  /**
   * @brief A certain complete subset of colors.
   *
   * The gamut, or color gamut, is a certain complete subset of colors. The most
   * common usage refers to the subset of colors which can be accurately
   * represented in a given circumstance, such as within a given color space or
   * by a certain output device.
   */
  enum GamutType {
    kGamutSRGB,
    kGamutAdobeRGB,
    kGamutDCIP3_D65,
    kGamutRec2020,
  };

  static ColorSpace MakeSRGB();

  static ColorSpace MakeSRGBLinear();

  ColorSpace(const ColorSpace &other);

  ColorSpace(ColorSpace &&other) noexcept;

  ColorSpace &operator=(const ColorSpace &other);

  ColorSpace &operator=(ColorSpace &&other) noexcept;

  ~ColorSpace();

  bool IsSRGB() const;

 private:

  struct Private;

  ColorSpace();

  std::unique_ptr<Private> p_;

};

bool operator==(const ColorSpace &src, const ColorSpace &dst);

bool operator!=(const ColorSpace &src, const ColorSpace &dst);

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHICS_COLOR_SPACE_HPP_
