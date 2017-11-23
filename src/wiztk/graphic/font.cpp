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

#include "internal/font_private.hpp"

#include "internal/typeface_private.hpp"
#include "internal/font-style_private.hpp"

namespace wiztk {
namespace graphic {

Font::Font(float size, MaskType mask_type, uint32_t flags) {
  p_ = std::make_unique<Private>();

  p_->sk_typeface = SkTypeface::MakeDefault();
  p_->sk_font = SkFont::Make(p_->sk_typeface, size, (SkFont::MaskType) mask_type, flags);
}

Font::Font(const char *family_name, const FontStyle &font_style, float size, MaskType mask_type, uint32_t flags) {
  p_ = std::make_unique<Private>();

  Typeface typeface(family_name, font_style);

  p_->sk_typeface = typeface.p_->sk_typeface;
  p_->sk_font = SkFont::Make(p_->sk_typeface, size, (SkFont::MaskType) mask_type, flags);
}

Font::Font(const Typeface &typeface, float size, MaskType mask_type, uint32_t flags) {
  p_ = std::make_unique<Private>();

  p_->sk_typeface = typeface.p_->sk_typeface;
  p_->sk_font = SkFont::Make(p_->sk_typeface, size, (SkFont::MaskType) mask_type, flags);
}

Font::Font(const Typeface &typeface, float size, float scale_x, float skew_x, MaskType mask_type, uint32_t flags) {
  p_ = std::make_unique<Private>();

  p_->sk_typeface = typeface.p_->sk_typeface;
  p_->sk_font =
      SkFont::Make(p_->sk_typeface, size, scale_x, skew_x, (SkFont::MaskType) mask_type, flags);
}

Font::Font(const Font &other) {
  p_ = std::make_unique<Private>();

  p_->sk_typeface = other.p_->sk_typeface;
  p_->sk_font = other.p_->sk_font;
}

Font::~Font() {

}

Font &Font::operator=(const Font &other) {
  p_->sk_font = other.p_->sk_font;
  return *this;
}

float Font::GetSize() const {
  return p_->sk_font->getSize();
}

float Font::GetScaleX() const {
  return p_->sk_font->getScaleX();
}

float Font::GetSkewX() const {
  return p_->sk_font->getSkewX();
}

uint32_t Font::GetFlags() const {
  return p_->sk_font->getFlags();
}

Font::MaskType Font::GetMaskType() const {
  return (MaskType) p_->sk_font->getMaskType();
}

bool Font::IsBold() const {
  return p_->sk_font->getTypeface()->isBold();
}

bool Font::IsItalic() const {
  return p_->sk_font->getTypeface()->isItalic();
}

bool Font::IsVertical() const {
  return p_->sk_font->isVertical();
}

bool Font::IsEmbolden() const {
  return p_->sk_font->isEmbolden();
}

bool Font::IsEnableAutoHints() const {
  return p_->sk_font->isEnableAutoHints();
}

bool Font::IsEnableByteCodeHints() const {
  return p_->sk_font->isEnableByteCodeHints();
}

bool Font::IsUseNonLinearMetrics() const {
  return p_->sk_font->isUseNonLinearMetrics();
}

bool Font::IsDevKern() {
  return p_->sk_font->isDevKern();
}

int Font::TextToGlyphs(const void *text,
                       size_t byte_length,
                       TextEncoding encoding,
                       GlyphID *glyphs,
                       int max_glyph_count) const {
  return p_->sk_font->textToGlyphs(text, byte_length, (SkTextEncoding) encoding, glyphs, max_glyph_count);
}

float Font::MeasureText(const void *text, size_t byte_length, TextEncoding encoding) const {
  return p_->sk_font->measureText(text, byte_length, (SkTextEncoding) encoding);
}

} // namespace graphic
} // namespace wiztk
