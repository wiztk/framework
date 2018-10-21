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

#include "surface/private.hpp"
#include "surface-props/private.hpp"

#include "canvas/private.hpp"
#include "image-info/private.hpp"

namespace wiztk {
namespace graphics {

Surface Surface::CreateRasterDirect(const ImageInfo &image_info,
                                    void *pixels,
                                    size_t row_bytes,
                                    const SurfaceProps *props) {
  Surface surface;

  surface.p_->sk_surface_sp =
      SkSurface::MakeRasterDirect(ImageInfo::Private::Get(image_info).sk_image_info,
                                  pixels,
                                  row_bytes,
                                  nullptr == props ? nullptr : &SurfaceProps::Private::Get(*props).sk_surface_props);

  if (!surface.p_->sk_surface_sp) {
    throw std::runtime_error("Error! Fail to create Surface object!");
  }

  return surface;
}

Surface Surface::CreateRaster(const ImageInfo &image_info,
                              size_t row_bytes,
                              const SurfaceProps *props) {
  Surface surface;

  surface.p_->sk_surface_sp =
      SkSurface::MakeRaster(ImageInfo::Private::Get(image_info).sk_image_info,
                            row_bytes,
                            nullptr == props ? nullptr : &SurfaceProps::Private::Get(*props).sk_surface_props);

  if (!surface.p_->sk_surface_sp) {
    throw std::runtime_error("Error! Fail to create Surface object!");
  }

  return surface;
}

Surface::Surface() {
  p_ = std::make_unique<Private>();
}

Surface::Surface(Surface &&other) noexcept {
  p_ = std::move(other.p_);
}

Surface::Surface(Private *p)
    : p_(p) {}

Surface::~Surface() = default;

Surface &Surface::operator=(Surface &&other) noexcept {
  p_ = std::move(other.p_);
  return *this;
}

} // namespace graphics
} // namespace wiztk
