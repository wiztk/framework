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

#ifndef WIZTK_GRAPHICS_TYPEFACE_PRIVATE_HPP_
#define WIZTK_GRAPHICS_TYPEFACE_PRIVATE_HPP_

#include <wiztk/graphics/typeface.hpp>

#include "SkTypeface.h"

namespace wiztk {
namespace graphics {

/**
 * @brief Structure to encapsulate a sk_sp<SkTypeface> object
 */
struct Typeface::Private {

  static const Private &Get(const Typeface &typeface) {
    return *typeface.p_;
  }

  Private() = default;

  explicit Private(const sk_sp<SkTypeface> &typeface)
      : sk_typeface_sp(typeface) {}

  Private(const Private &) = default;

  Private &operator=(const Private &other) = default;

  Private &operator=(const sk_sp<SkTypeface> &typeface) {
    sk_typeface_sp = typeface;
    return *this;
  }

  sk_sp<SkTypeface> sk_typeface_sp;

};

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHIC_TYPEFACE_PRIVATE_HPP_
