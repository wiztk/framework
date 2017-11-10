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

#ifndef WIZTK_GRAPHIC_INTERNAL_SURFACE_PRIVATE_HPP_
#define WIZTK_GRAPHIC_INTERNAL_SURFACE_PRIVATE_HPP_

#include "wiztk/graphic/surface.hpp"

#include "SkSurface.h"

namespace wiztk {
namespace graphic {

struct Surface::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private()
      : canvas(nullptr) {}

  ~Private() {}

  Canvas *canvas;
  sk_sp<SkSurface> sp_sk_surface;

};

} // namespace graphic
} // namespace wiztk

#endif // WIZTK_GRAPHIC_INTERNAL_LAYER_PRIVATE_HPP_
