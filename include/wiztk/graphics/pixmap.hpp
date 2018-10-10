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

#ifndef WIZTK_GRAPHICS_PIXMAP_HPP_
#define WIZTK_GRAPHICS_PIXMAP_HPP_

#include "wiztk/base/macros.hpp"

#include "wiztk/graphics/image-info.hpp"

#include <memory>

namespace wiztk {
namespace graphics {

class WIZTK_EXPORT Pixmap {

 public:

  struct Private;

  Pixmap();

  Pixmap(const ImageInfo &info, const void *addr, size_t row_bytes);

  virtual ~Pixmap();

  void Reset();

  void Reset(const ImageInfo &info, const void *addr, size_t row_bytes);

  void SetColorSpace(const ColorSpace &color_space);

  size_t GetRowBytes() const;

  const void *GetAddr() const;

  int GetWidth() const;

  int GetHeight() const;

  ColorType GetColorType() const;

  AlphaType GetAlphaType() const;

 private:

  std::unique_ptr<Private> p_;

};

}
}

#endif // WIZTK_GRAPHICS_PIXMAP_HPP_
