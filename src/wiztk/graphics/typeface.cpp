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

#include "typeface/private.hpp"
#include "font-style/private.hpp"

namespace wiztk {
namespace graphics {

using base::RectF;

Typeface::Typeface() {
  p_ = std::make_unique<Private>();
  p_->sk_typeface_sp = SkTypeface::MakeDefault();
}

Typeface::Typeface(const char *family_name, const FontStyle &font_style) {
  p_ = std::make_unique<Private>();
  p_->sk_typeface_sp = SkTypeface::MakeFromName(family_name,
      FontStyle::Private::Get(font_style).sk_font_style);
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
  p_->sk_typeface_sp = SkTypeface::MakeFromFile(path, index);
}

Typeface::Typeface(const Typeface &other) {
  p_ = std::make_unique<Private>();
  p_->sk_typeface_sp = other.p_->sk_typeface_sp;
}

Typeface::~Typeface() = default;

Typeface &Typeface::operator=(const Typeface &other) {
  p_->sk_typeface_sp = other.p_->sk_typeface_sp;
  return *this;
}

FontStyle Typeface::GetFontStyle() const {
  return FontStyle(FontStyle::Private(p_->sk_typeface_sp->fontStyle()));
}

bool Typeface::IsBold() const {
  return p_->sk_typeface_sp->isBold();
}

bool Typeface::IsItalic() const {
  return p_->sk_typeface_sp->isItalic();
}

bool Typeface::IsFixedPitch() const {
  return p_->sk_typeface_sp->isFixedPitch();
}

FontID Typeface::GetUniqueID() const {
  return p_->sk_typeface_sp->uniqueID();
}

int Typeface::CharsToGlyphs(const void *chars, Encoding encoding, GlyphID *glyphs, int glyph_count) const {
  return p_->sk_typeface_sp->charsToGlyphs(chars, (SkTypeface::Encoding) encoding, glyphs, glyph_count);
}

int Typeface::CountGlyphs() const {
  return p_->sk_typeface_sp->countGlyphs();
}

int Typeface::CountTables() const {
  return p_->sk_typeface_sp->countTables();
}

int Typeface::GetTableTags(FontTableTag *tags) const {
  return p_->sk_typeface_sp->getTableTags(tags);
}

size_t Typeface::GetTableSize(FontTableTag tag) const {
  return p_->sk_typeface_sp->getTableSize(tag);
}

size_t Typeface::GetTableData(FontTableTag tag, size_t offset, size_t lengh, void *data) const {
  return p_->sk_typeface_sp->getTableData(tag, offset, lengh, data);
}

int Typeface::GetUnitsPerEm() const {
  return p_->sk_typeface_sp->getUnitsPerEm();
}

bool Typeface::GetKerningPairAdjustments(const GlyphID *glyphs, int count, int32_t *adjustments) const {
  return p_->sk_typeface_sp->getKerningPairAdjustments(glyphs, count, adjustments);
}

RectF Typeface::GetBounds() const {
  SkRect rect = p_->sk_typeface_sp->getBounds();
  return *reinterpret_cast<RectF *>(&rect);
}

bool operator==(const Typeface &typeface1, const Typeface &typeface2) {
  return SkTypeface::Equal(typeface1.p_->sk_typeface_sp.get(), typeface2.p_->sk_typeface_sp.get());
}

} // namespace graphics
} // namespace wiztk
