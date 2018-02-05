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

#include "typeface_private.hpp"
#include "font-style_private.hpp"

namespace wiztk {
namespace graphic {

using base::RectF;

Typeface::Typeface() {
  p_ = std::make_unique<Private>();
  p_->sk_typeface = SkTypeface::MakeDefault();
}

Typeface::Typeface(const char *family_name, const FontStyle &font_style) {
  p_ = std::make_unique<Private>();
  SkFontStyle style = font_style.p_->sk_font_style;
  p_->sk_typeface = SkTypeface::MakeFromName(family_name, style);
}

//Typeface::Typeface(const Typeface &other, Style style) {
//  p_ = std::make_unique<Private>();
//  p_->sk_typeface = SkTypeface::MakeFromTypeface(other.p_->sk_typeface.get(), (SkTypeface::Style) style);
//}
//
//Typeface::Typeface(SkTypeface *family, Style style) {
//  p_ = std::make_unique<Private>();
//  p_->sk_typeface = SkTypeface::MakeFromTypeface(family, (SkTypeface::Style) style);
//}

Typeface::Typeface(const char *path, int index) {
  p_ = std::make_unique<Private>();
  p_->sk_typeface = SkTypeface::MakeFromFile(path, index);
}

Typeface::Typeface(const Typeface &other) {
  p_ = std::make_unique<Private>();
  p_->sk_typeface = other.p_->sk_typeface;
}

Typeface::~Typeface() {

}

Typeface &Typeface::operator=(const Typeface &other) {
  p_->sk_typeface = other.p_->sk_typeface;
  return *this;
}

//FontStyle Typeface::GetFontStyle() const {
//  return FontStyle(p_->sk_typeface->fontStyle());
//}

bool Typeface::IsBold() const {
  return p_->sk_typeface->isBold();
}

bool Typeface::IsItalic() const {
  return p_->sk_typeface->isItalic();
}

bool Typeface::IsFixedPitch() const {
  return p_->sk_typeface->isFixedPitch();
}

FontID Typeface::GetUniqueID() const {
  return p_->sk_typeface->uniqueID();
}

int Typeface::CharsToGlyphs(const void *chars, Encoding encoding, GlyphID *glyphs, int glyph_count) const {
  return p_->sk_typeface->charsToGlyphs(chars, (SkTypeface::Encoding) encoding, glyphs, glyph_count);
}

int Typeface::CountGlyphs() const {
  return p_->sk_typeface->countGlyphs();
}

int Typeface::CountTables() const {
  return p_->sk_typeface->countTables();
}

int Typeface::GetTableTags(FontTableTag *tags) const {
  return p_->sk_typeface->getTableTags(tags);
}

size_t Typeface::GetTableSize(FontTableTag tag) const {
  return p_->sk_typeface->getTableSize(tag);
}

size_t Typeface::GetTableData(FontTableTag tag, size_t offset, size_t lengh, void *data) const {
  return p_->sk_typeface->getTableData(tag, offset, lengh, data);
}

int Typeface::GetUnitsPerEm() const {
  return p_->sk_typeface->getUnitsPerEm();
}

bool Typeface::GetKerningPairAdjustments(const GlyphID *glyphs, int count, int32_t *adjustments) const {
  return p_->sk_typeface->getKerningPairAdjustments(glyphs, count, adjustments);
}

RectF Typeface::GetBounds() const {
  SkRect rect = p_->sk_typeface->getBounds();
  return *reinterpret_cast<RectF *>(&rect);
}

bool operator==(const Typeface &typeface1, const Typeface &typeface2) {
  return SkTypeface::Equal(typeface1.p_->sk_typeface.get(), typeface2.p_->sk_typeface.get());
}

} // namespace graphic
} // namespace wiztk
