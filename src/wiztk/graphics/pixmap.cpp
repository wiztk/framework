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

#include "pixmap/private.hpp"

#include "image-info/private.hpp"
#include "color-space/private.hpp"

#include "SkPixmap.h"

namespace wiztk {
namespace graphics {

Pixmap::Pixmap() {
  p_ = std::make_unique<Private>();
}

Pixmap::Pixmap(const ImageInfo &info, const void *addr, size_t row_bytes) {
  p_ = std::make_unique<Private>(ImageInfo::Private::Get(info).sk_image_info, addr, row_bytes);
}

Pixmap::~Pixmap() = default;

void Pixmap::Reset() {
  p_->sk_pixmap.reset();
}

void Pixmap::Reset(const ImageInfo &info, const void *addr, size_t row_bytes) {
  p_->sk_pixmap.reset(ImageInfo::Private::Get(info).sk_image_info, addr, row_bytes);
}

void Pixmap::SetColorSpace(const ColorSpace &color_space) {
  p_->sk_pixmap.setColorSpace(ColorSpace::Private::Get(color_space).sk_sp_color_space);
}

} // namespace graphics
} // namespace wiztk