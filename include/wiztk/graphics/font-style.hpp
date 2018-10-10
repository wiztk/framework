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

#ifndef WIZTK_GRAPHIC_FONT_STYLE_HPP_
#define WIZTK_GRAPHIC_FONT_STYLE_HPP_

#include "wiztk/base/macros.hpp"

#include <memory>

namespace wiztk {
namespace graphics {

/**
 * @ingroup graphics
 * @brief Font style
 *
 * This class has the same structure of SkFontStyle so it can be reinterpreted cast to each other.
 */
class WIZTK_EXPORT FontStyle {

  friend class Typeface;

 public:

  struct Private;

  enum Weight {
    kWeightInvisible = 0,
    kWeightThin = 100,
    kWeightExtraLight = 200,
    kWeightLight = 300,
    kWeightNormal = 400,
    kWeightMedium = 500,
    kWeightSemiBold = 600,
    kWeightBold = 700,
    kWeightExtraBold = 800,
    kWeightBlack = 900,
    kWeightExtraBlack = 1000,
  };

  enum Width {
    kWidthUltraCondensed = 1,
    kWidthExtraCondensed = 2,
    kWidthCondensed = 3,
    kWidthSemiCondensed = 4,
    kWidthNormal = 5,
    kWidthSemiExpanded = 6,
    kWidthExpanded = 7,
    kWidthExtraExpanded = 8,
    kWidthUltraExpanded = 9,
  };

  enum Slant {
    kSlantUpright,
    kSlantItalic,
    kSlantOblique
  };

  explicit FontStyle(int weight = kWeightNormal, int width = kWidthNormal, Slant slant = kSlantUpright);

  FontStyle(const FontStyle &other);

  /**
   * @brief Move constructor
   * @param other
   */
  FontStyle(FontStyle &&other) noexcept;

  FontStyle &operator=(const FontStyle &other);

  FontStyle &operator=(FontStyle &&other) noexcept;

  ~FontStyle();

  bool operator==(const FontStyle &other) const;

  int GetWeight() const;

  int GetWidth() const;

  Slant GetSlant() const;

 private:

  explicit FontStyle(const Private &p);

  std::unique_ptr<Private> p_;

};

} // namespace graphics
} // namespace wiztk


#endif // WIZTK_GRAPHIC_FONT_STYLE_HPP_
