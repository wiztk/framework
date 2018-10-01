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

#include "image-info/private.hpp"
#include "color-space/private.hpp"

namespace wiztk {
namespace graphics {

ImageInfo ImageInfo::Make(int width, int height, ColorType ct, AlphaType at) {
  ImageInfo image_info;
  image_info.p_->sk_image_info =
      SkImageInfo::Make(width, height,
                        static_cast<SkColorType>(ct),
                        static_cast<SkAlphaType>(at));
  return image_info;
}

ImageInfo ImageInfo::Make(int width,
                          int height,
                          wiztk::graphics::ColorType ct,
                          wiztk::graphics::AlphaType at,
                          const wiztk::graphics::ColorSpace &cs) {
  ImageInfo image_info;
  image_info.p_->sk_image_info =
      SkImageInfo::Make(width, height,
                        static_cast<SkColorType>(ct),
                        static_cast<SkAlphaType>(at),
                        cs.p_->sk_sp_color_space);
  return image_info;
}

ImageInfo ImageInfo::MakeN32Premul(int width, int height) {
  ImageInfo image_info;
  image_info.p_->sk_image_info = SkImageInfo::MakeN32Premul(width, height);
  return image_info;
}

ImageInfo ImageInfo::MakeN32Premul(int width, int height, const wiztk::graphics::ColorSpace &cs) {
  ImageInfo image_info;
  image_info.p_->sk_image_info = SkImageInfo::MakeN32Premul(width, height, cs.p_->sk_sp_color_space);
  return image_info;
}

ImageInfo::ImageInfo() {
  p_ = std::make_unique<Private>();
}

ImageInfo::ImageInfo(const ImageInfo &orig) {
  p_ = std::make_unique<Private>(*orig.p_);
}

ImageInfo::ImageInfo(wiztk::graphics::ImageInfo &&other) noexcept {
  p_ = std::move(other.p_);
}

ImageInfo::~ImageInfo() = default;

ImageInfo &ImageInfo::operator=(const ImageInfo &other) {
  (*p_) = (*other.p_);
  return *this;
}

ImageInfo &ImageInfo::operator=(ImageInfo &&other) noexcept {
  p_ = std::move(other.p_);
  return *this;
}

int ImageInfo::GetWidth() const {
  return p_->sk_image_info.width();
}

int ImageInfo::GetHeight() const {
  return p_->sk_image_info.height();
}

ColorType ImageInfo::GetColorType() const {
  return static_cast<ColorType>(p_->sk_image_info.colorType());
}

AlphaType ImageInfo::GetAlphaType() const {
  return static_cast<AlphaType>(p_->sk_image_info.alphaType());
}

void ImageInfo::Reset() {
  p_->sk_image_info.reset();
}

} // namespace graphics
} // namespace wiztk
