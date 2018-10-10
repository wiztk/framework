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

#include "font-style/private.hpp"

namespace wiztk {
namespace graphics {

FontStyle::FontStyle(int weight, int width, Slant slant) {
  p_ = std::make_unique<Private>(weight, width, static_cast<SkFontStyle::Slant>(slant));
}

FontStyle::FontStyle(const FontStyle &other) {
  p_->sk_font_style = other.p_->sk_font_style;
}

FontStyle::FontStyle(const Private &p) {
  p_ = std::make_unique<Private>(p);
}

FontStyle::FontStyle(FontStyle &&other) noexcept {
  p_ = std::move(other.p_);
}

FontStyle &FontStyle::operator=(const FontStyle &other) {
  p_->sk_font_style = other.p_->sk_font_style;
  return *this;
}

FontStyle &FontStyle::operator=(FontStyle &&other) noexcept {
  p_ = std::move(other.p_);
  return *this;
}

FontStyle::~FontStyle() = default;

bool FontStyle::operator==(const FontStyle &other) const {
  return p_->sk_font_style == other.p_->sk_font_style;
}

int FontStyle::GetWeight() const {
  return p_->sk_font_style.weight();
}

int FontStyle::GetWidth() const {
  return p_->sk_font_style.width();
}

FontStyle::Slant FontStyle::GetSlant() const {
  return static_cast<Slant >(p_->sk_font_style.slant());
}

} // namespace graphics
} // namespace wiztk
