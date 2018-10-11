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

#ifndef WIZTK_GRAPHICS_IMAGE_PRIVATE_HPP_
#define WIZTK_GRAPHICS_IMAGE_PRIVATE_HPP_

#include "wiztk/graphics/image.hpp"

#include "SkImage.h"

namespace wiztk {
namespace graphics {

struct Image::Private {

  static const Private &Get(const Image &image) {
    return *image.p_;
  }

  Private() = default;

  Private(const Private &) = default;

  Private(Private &&other) noexcept {
    sk_image_sp = std::move(other.sk_image_sp);
  }

  ~Private() = default;

  Private &operator=(const Private &) = default;

  Private &operator=(Private &&other) noexcept {
    sk_image_sp = std::move(other.sk_image_sp);
    return *this;
  }

  sk_sp<SkImage> sk_image_sp;

};

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHICS_IMAGE_PRIVATE_HPP_
