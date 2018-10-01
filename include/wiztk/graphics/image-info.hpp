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

#ifndef WIZTK_GRAPHICS_IMAGE_INFO_HPP_
#define WIZTK_GRAPHICS_IMAGE_INFO_HPP_

#include "wiztk/base/macros.hpp"

#include "wiztk/graphics/color-space.hpp"

#include <memory>

namespace wiztk {
namespace graphics {

/**
 * @ingroup graphics
 * @brief Describes how to interpret the alpha component of a pixel
 */
enum AlphaType {
  kAlphaTypeUnknown,                    ///< Uninitialized
  kAlphaTypeOpaque,                     ///< Pixel is opaque
  kAlphaTypePremul,
  kAlphaTypeUnpremul,
  kAlphaTypeLast = kAlphaTypeUnpremul
};

/**
 * @ingroup graphics
 * @brief Describes how to interpret the components of a pixel
 */
enum ColorType {
  kColorTypeUnknown,

  kColorTypeAlpha8,
  kColorTypeRGB565,
  kColorTypeARGB4444,
  kColorTypeRGBA8888,
  kColorTypeRGB888x,
  kColorTypeBGRA8888,
  kColorTypeRGBA1010102,
  kColorTypeRGB101010x,
  kColorTypeGray8,
  kColorTypeRGBAF16,
  kColorTypeRGBAF32,

  kColorTypeLast = kColorTypeRGBAF32
};

/**
 * @ingroup graphics
 * @brief Describes pixel dimensions and encoding
 */
class WIZTK_EXPORT ImageInfo {

  friend class Bitmap;
  friend class ViewSurface;
  friend class Canvas;

 public:

  static ImageInfo Make(int width, int height, ColorType ct, AlphaType at);

  static ImageInfo Make(int width, int height, ColorType ct, AlphaType at, const ColorSpace &cs);

  static ImageInfo MakeN32Premul(int width, int height);

  static ImageInfo MakeN32Premul(int width, int height, const ColorSpace &cs);

  ImageInfo(const ImageInfo &other);

  ImageInfo(ImageInfo &&other) noexcept;

  ~ImageInfo();

  ImageInfo &operator=(const ImageInfo &other);

  ImageInfo &operator=(ImageInfo &&other) noexcept;

  int GetWidth() const;

  int GetHeight() const;

  ColorType GetColorType() const;

  AlphaType GetAlphaType() const;

  void Reset();

 private:

  struct Private;

  ImageInfo();

  std::unique_ptr<Private> p_;

};

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHICS_IMAGE_INFO_HPP_
