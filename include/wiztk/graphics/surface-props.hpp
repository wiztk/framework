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

#ifndef WIZTK_GRAPHIC_SURFACE_PROPS_HPP_
#define WIZTK_GRAPHIC_SURFACE_PROPS_HPP_

#include <memory>

namespace wiztk {
namespace graphics {

enum PixelGeometryType {
  kPixelGeometryUnknown,
  kPixelGeometry_RGB_H,
  kPixelGeometry_BGR_H,
  kPixelGeometry_RGB_V,
  kPixelGeometry_BGR_V
};

class SurfaceProps {

 public:

  SurfaceProps(const SurfaceProps &other) = delete;
  SurfaceProps &operator=(const SurfaceProps &other) = delete;

  enum InitType {
    kInitLegacyFontHost
  };

  struct Private;

  SurfaceProps();

  explicit SurfaceProps(uint32_t flags);

  SurfaceProps(SurfaceProps &&other) noexcept;

  ~SurfaceProps();

  SurfaceProps &operator=(SurfaceProps &&other) noexcept;

 private:

  std::unique_ptr<Private> p_;

};

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHIC_SURFACE_PROPS_HPP_
