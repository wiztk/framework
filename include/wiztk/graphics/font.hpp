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

#ifndef WIZTK_GRAPHIC_FONT_HPP_
#define WIZTK_GRAPHIC_FONT_HPP_

#include "typeface.hpp"

#include <memory>

namespace wiztk {
namespace graphics {

/**
 * @ingroup graphic
 * @brief Text encoding
 */
enum TextEncoding {
  kTextEncodingUTF8,
  kTextEncodingUTF16,
  kTextEncodingUTF32,
  kTextEncodingGlyphID
};

/**
 * @ingroup graphic
 * @brief Font
 */
class Font {

  friend class Paint;

 public:

  enum Flags {
    kFlagEnableAutoHints = 1 << 0,
    kFlagEnableByteCodeHints = 1 << 1,
    kFlagEmbeddedBitmaps = 1 << 2,
    kFlagUseNonlinearMetrics = 1 << 3,

    kFlagVertical = 1 << 4,
    kFlagGenA8FromLCD = 1 << 5,
    kFlagEmbolden = 1 << 6,
    kFlagDevKern = 1 << 7
  };

  enum MaskType {
    kMaskBW,
    kMaskA8,
    kMaskLCD
  };

  /**
   * @brief Create default font
   * @param style An enum value of Typeface::Style
   * @param size
   * @param mask_type
   * @param flags
   */
  Font(float size = 12.f,
       MaskType mask_type = kMaskBW,
       uint32_t flags = kFlagEnableAutoHints);

  Font(const char *family_name,
       const FontStyle &font_style,
       float size,
       MaskType mask_type = kMaskBW,
       uint32_t flags = kFlagEnableAutoHints);

  Font(const Typeface &typeface,
       float size,
       MaskType mask_type = kMaskBW,
       uint32_t flags = kFlagEnableAutoHints);

  Font(const Typeface &typeface,
       float size,
       float scale_x,
       float skew_x,
       MaskType mask_type = kMaskBW,
       uint32_t flags = kFlagEnableAutoHints);

  Font(const Font &other);

  ~Font();

  Font &operator=(const Font &other);

  float GetSize() const;

  float GetScaleX() const;

  float GetSkewX() const;

  uint32_t GetFlags() const;

  MaskType GetMaskType() const;

  bool IsBold() const;

  bool IsItalic() const;

  bool IsVertical() const;

  bool IsEmbolden() const;

  bool IsEnableAutoHints() const;

  bool IsEnableByteCodeHints() const;

  bool IsUseNonLinearMetrics() const;

  bool IsDevKern();

  int TextToGlyphs(const void *text,
                   size_t byte_length,
                   TextEncoding encoding,
                   GlyphID glyphs[],
                   int max_glyph_count) const;

  int CountText(const void *text, size_t byte_length, TextEncoding encoding) {
    return TextToGlyphs(text, byte_length, encoding, nullptr, 0);
  }

  float MeasureText(const void *text, size_t byte_length, TextEncoding encoding) const;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHIC_FONT_HPP_
