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

#include "wiztk/graphics/image.hpp"

#include "SkImage.h"

namespace wiztk {
namespace graphics {

struct Image::Private {

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

// ----

Image Image::MakeFromRaster() {
  Image image;

  return image;
}

Image::Image() {
  p_ = std::make_unique<Private>();
}

Image::Image(const Image &other) {
  p_ = std::make_unique<Private>(*other.p_);
}

Image::Image(Image &&other) noexcept {
  p_ = std::move(other.p_);
}

Image::~Image() = default;

Image &Image::operator=(const Image &other) {
  *p_ = *other.p_;
  return *this;
}

Image &Image::operator=(Image &&other) noexcept {
  p_ = std::move(other.p_);
  return *this;
}

} // namespace graphics
} // namespace wiztk
