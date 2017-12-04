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

#include "internal/surface_private.hpp"

#include "internal/canvas_private.hpp"
#include "internal/image-info_private.hpp"

namespace wiztk {
namespace graphic {

Surface *Surface::CreateRasterDirect(const ImageInfo &image_info, void *pixels, size_t row_bytes) {
  Surface *surface = new Surface();

//  surface->p_->sp_sk_surface =
//      SkSurface::MakeRasterDirect(SkImageInfo::Make(image_info.width(), image_info.height(),
//                                                    static_cast<SkColorType >(image_info.color_type()),
//                                                    static_cast<SkAlphaType>(image_info.alpha_type())),
//                                  pixels,
//                                  row_bytes);
//  if (!surface->p_->sp_sk_surface) {
//    throw std::runtime_error("Error! Fail to create Surface object!");
//  }
//
//  surface->p_->canvas = new Canvas(surface->p_->sp_sk_surface->getCanvas());

  return surface;
}

Surface::Surface() {
  p_ = std::make_unique<Private>();
}

Surface::~Surface() {
  // The SkCanvas pointer is destroyed in SkSurface
//  p_->canvas->p_->sk_canvas = nullptr;
}

} // namespace graphic
} // namespace wiztk
