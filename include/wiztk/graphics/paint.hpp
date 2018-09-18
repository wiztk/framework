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

#ifndef WIZTK_GRAPHIC_PAINT_HPP_
#define WIZTK_GRAPHIC_PAINT_HPP_

#include "wiztk/base/color.hpp"

#include "wiztk/graphics/font.hpp"
#include "wiztk/graphics/alignment.hpp"

#include <cstdint>
#include <memory>

class SkPaint;

namespace wiztk {
namespace graphics {

class Shader;

/**
 * @ingroup graphics
 * @brief Class holds the style and color information about how to draw geometries, text and bitmaps.
 */
class WIZTK_EXPORT Paint {

  friend bool operator==(const Paint &paint1, const Paint &paint2);
  friend bool operator!=(const Paint &paint1, const Paint &paint2);

 public:

  using RectF = base::RectF;
  using ColorF = base::ColorF;

 public:

  enum Style {
    kStyleFill,   //!< fill the geometry
    kStyleStroke,      //!< stroke the geometry
    kStyleStrokeAndFill    //!< fill and stroke the geometry
  };

  enum Hinting {
    kHintingNone,
    kHintingSlight,
    kHintingNormal,
    kHintingFull
  };

  Paint();

  Paint(const Paint &orig);

  ~Paint();

  Paint &operator=(const Paint &other);

  uint32_t GetHash() const;

  void Reset();

  Hinting GetHinting() const;

  void SetHinting(Hinting hinting);

  bool IsAntiAlias() const;

  void SetAntiAlias(bool aa);

  void SetStyle(Style style);

  base::ColorF GetColor() const;

  void SetColor(uint32_t argb);

  void SetColor(const ColorF &color);

  float GetStrokeWidth() const;

  void SetStrokeWidth(float width);

  void SetFont(const Font &font);

  void SetShader(const Shader &shader);

  Alignment::Horizontal GetTextAlign() const;

  void SetTextAlign(Alignment::Horizontal align);

  float GetTextSize() const;

  void SetTextSize(float size);

  float GetTextScaleX() const;

  void SetTextScaleX(float scale_x);

  float GetTextSkewX() const;

  void SetTextSkewX(float skew_x);

  TextEncoding GetTextEncoding() const;

  void SetTextEncoding(TextEncoding encoding);

  int TextToGlyphs(const void *text, size_t byte_length, uint16_t glyphs[]) const;

  bool ContainsText(const void *text, size_t byte_length) const;

  void GlyphsToUnichars(const uint16_t glyphs[], int count, int32_t text[]) const;

  int CountText(const void *text, size_t byte_length) const;

  float MeasureText(const void *text, size_t length, RectF *boulds) const;

  float MeasureText(const void *text, size_t length) const {
    return MeasureText(text, length, nullptr);
  }

  const SkPaint &GetSkPaint() const;

 private:

  struct Private;

  std::unique_ptr<Private> p_;
};

bool operator==(const Paint &paint1, const Paint &paint2);

bool operator!=(const Paint &paint1, const Paint &paint2);

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHIC_PAINT_HPP_
