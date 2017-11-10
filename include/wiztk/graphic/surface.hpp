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

#ifndef WIZTK_GRAPHIC_SURFACE_HPP_
#define WIZTK_GRAPHIC_SURFACE_HPP_

#include <memory>

namespace wiztk {
namespace graphic {

// Forward declarations
class Canvas;
class ImageInfo;

/**
 * @ingroup graphic
 * @brief A class responsible for managing the pixels that a canvas draws into
 *
 */
class Surface {

  Surface(const Surface &) = delete;
  Surface &operator=(const Surface &) = delete;

 public:

  /**
   * @brief Create a new surface object, using the specified pixels/rowbytes as its backend
   * @param pixels
   * @param row_bytes
   * @return
   */
  static Surface *CreateRasterDirect(const ImageInfo &, void *pixels, size_t row_bytes);

  static Surface *CreateRaster(const ImageInfo &, size_t row_types);

  virtual ~Surface();

  Canvas *GetCanvas() const;

 protected:

  Surface();

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

} // namespace graphic
} // namespace wiztk

#endif //WIZTK_LAYER_HPP
