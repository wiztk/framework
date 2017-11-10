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

#ifndef WIZTK_GRAPHIC_INTERNAL_FONT_PRIVATE_HPP_
#define WIZTK_GRAPHIC_INTERNAL_FONT_PRIVATE_HPP_

#include <wiztk/graphic/font.hpp>

#include "SkFont.h"

namespace wiztk {
namespace graphic {

/**
 * @ingroup graphic_intern
 * @brief Structure to encapsulate a sk_sp<SkFont> object
 */
struct Font::Private {

  Private() = default;

  explicit Private(const sk_sp<SkFont> &font)
      : sk_font(font) {}

  Private(const Private &other)
      : sk_font(other.sk_font) {}

  Private &operator=(const Private &other) {
    sk_font = other.sk_font;
    return *this;
  }

  Private &operator=(const sk_sp<SkFont> &font) {
    sk_font = font;
    return *this;
  }

  sk_sp<SkFont> sk_font;
  sk_sp<SkTypeface> sk_typeface;

};

} // namespace graphic
} // namespace wiztk

#endif // WIZTK_GRAPHIC_INTERNAL_FONT_META_HPP_
