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

#include "color-space/private.hpp"

namespace wiztk {
namespace graphics {

ColorSpace::ColorSpace() {
  p_ = std::make_unique<Private>();
}

ColorSpace::ColorSpace(const ColorSpace &other) {
  p_ = std::make_unique<Private>(*other.p_);
}

ColorSpace::ColorSpace(ColorSpace &&other) noexcept {
  p_ = std::move(other.p_);
}

ColorSpace &ColorSpace::operator=(const ColorSpace &other) {
  (*p_) = (*other.p_);
  return *this;
}

ColorSpace &ColorSpace::operator=(ColorSpace &&other) noexcept {
  p_ = std::move(other.p_);
  return *this;
}

ColorSpace::~ColorSpace() = default;

ColorSpace ColorSpace::MakeSRGB() {
  ColorSpace color_space;
  color_space.p_->sk_sp_color_space = SkColorSpace::MakeSRGB();
  return color_space;
}

ColorSpace ColorSpace::MakeSRGBLinear() {
  ColorSpace color_space;
  color_space.p_->sk_sp_color_space = SkColorSpace::MakeSRGBLinear();
  return color_space;
}

bool ColorSpace::IsSRGB() const {
  return p_->sk_sp_color_space->isSRGB();
}

bool operator==(const ColorSpace &src, const ColorSpace &dst) {
  return SkColorSpace::Equals(src.p_->sk_sp_color_space.get(), dst.p_->sk_sp_color_space.get());
}

bool operator!=(const ColorSpace &src, const ColorSpace &dst) {
  return !SkColorSpace::Equals(src.p_->sk_sp_color_space.get(), dst.p_->sk_sp_color_space.get());
}

} // namespace graphics
} // namespace wiztk