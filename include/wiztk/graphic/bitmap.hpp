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

#ifndef WIZTK_GRAPHIC_BITMAP_HPP_
#define WIZTK_GRAPHIC_BITMAP_HPP_

#include <memory>
#include "image-info.hpp"

namespace wiztk {
namespace graphic {

/**
 * @ingroup graphic
 * @brief A Bitmap class represents a raster bitmap.
 */
class Bitmap {

  friend class Canvas;

 public:

  /**
   * @brief Default constructor
   *
   * The default constructor creates a bitmap with zero width and height, and no
   * pixels. Its color type is set to unknown.
   */
  Bitmap();

  Bitmap(const Bitmap &orig);

  Bitmap &operator=(const Bitmap &other);

  ~Bitmap();

  void AllocatePixels(const ImageInfo &info, size_t row_bytes);

  void AllocatePixels(const ImageInfo &info);

  void AllocateN32Pixels(int width, int height, bool is_opaque = false);

  bool InstallPixels(const ImageInfo &info, void *pixels, size_t row_bytes);

  void WriteToFile(const std::string &filename) const;

  int GetWidth() const;

  int GetHeight() const;

  ColorType GetColorType() const;

  AlphaType GetAlphaType() const;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

} // namespace graphic
} // namespace wiztk

#endif // WIZTK_GRAPHIC_BITMAP_HPP_
