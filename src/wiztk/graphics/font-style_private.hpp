/*
 * Copyright 2017 Freeman Zhang <zhanggyb@gmail.com>
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

#ifndef WIZTK_GRAPHIC_FONT_STYLE_PRIVATE_HPP_
#define WIZTK_GRAPHIC_FONT_STYLE_PRIVATE_HPP_

#include "wiztk/graphics/font-style.hpp"

#include "SkFontStyle.h"

namespace wiztk {
namespace graphics {

struct FontStyle::Private {

  Private() = default;

  explicit Private(const SkFontStyle &native)
      : sk_font_style(native) {}

  Private(const Private &other) = default;

  Private(int weight, int width, SkFontStyle::Slant slant)
      : sk_font_style(weight, width, slant) {}

  ~Private() = default;

  Private &operator=(const Private &other) = default;

  SkFontStyle sk_font_style;

};

}
}

#endif // WIZTK_GRAPHIC_INTERNAL_FONT_STYLE_PRIVATE_HPP_
