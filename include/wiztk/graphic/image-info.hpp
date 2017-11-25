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

#ifndef WIZTK_GRAPHIC_IMAGE_INFO_HPP_
#define WIZTK_GRAPHIC_IMAGE_INFO_HPP_

namespace wiztk {
namespace graphic {

/**
 * @ingroup graphic
 * @brief Describes how to interpret the alpha component of a pixel
 */
enum AlphaType {
  kAlphaTypeUnknown,

  kAlphaTypeOpaque,

  kAlphaTypePremul,

  kAlphaTypeUnpremul,

  kAlphaTypeLast = kAlphaTypeUnpremul
};

/**
 * @ingroup graphic
 * @brief Describes how to interpret the components of a pixel
 */
enum ColorType {
  kColorTypeUnknown,

  kColorTypeAlpha8,
  kColorTypeRGB565,
  kColorTypeARGB4444,
  kColorTypeRGBA8888,
  kColorTypeBGRA8888,
  kColorTypeIndex8,
  kColorTypeGray8,
  kColorTypeRGBAF16,

  kColorTypeLast = kColorTypeRGBAF16
};

/**
 * @ingroup graphic
 * @brief Describes an images' dimensions and pixel type
 */
class ImageInfo {

  friend class Bitmap;
  friend class ViewSurface;
  friend class Canvas;

 public:

  static ImageInfo Make(int width, int height, ColorType ct, AlphaType at) {
    return ImageInfo(width, height, ct, at);
  }

  static ImageInfo MakeN32Premul(int width, int height) {
    return ImageInfo(width, height, kColorTypeBGRA8888, kAlphaTypePremul);
  }

  ImageInfo() = default;

  ImageInfo(const ImageInfo &orig) = default;

  ~ImageInfo() = default;

  ImageInfo &operator=(const ImageInfo &other) = default;

  int width() const { return width_; }

  int height() const { return height_; }

  ColorType color_type() const { return color_type_; }

  AlphaType alpha_type() const { return alpha_type_; }

  void reset() {
    width_ = 0;
    height_ = 0;
    color_type_ = kColorTypeUnknown;
    alpha_type_ = kAlphaTypeUnknown;
  }

 private:

  ImageInfo(int width, int height, ColorType color_type, AlphaType alpha_type)
      : width_(width),
        height_(height),
        color_type_(color_type),
        alpha_type_(alpha_type) {}

  int width_ = 0;
  int height_ = 0;
  ColorType color_type_ = kColorTypeUnknown;
  AlphaType alpha_type_ = kAlphaTypeUnknown;

};

} // namespace graphic
} // namespace wiztk

#endif // WIZTK_GRAPHIC_IMAGE_INFO_HPP_
