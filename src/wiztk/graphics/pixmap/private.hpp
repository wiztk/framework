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

#ifndef WIZTK_GRAPHICS_PIXMAP_PRIVATE_HPP_
#define WIZTK_GRAPHICS_PIXMAP_PRIVATE_HPP_

#include "wiztk/graphics/pixmap.hpp"

#include "SkImageInfo.h"
#include "SkPixmap.h"

namespace wiztk {
namespace graphics {

struct Pixmap::Private {

  static const Private &Get(const Pixmap &pixmap) {
    return *pixmap.p_;
  }

  Private() = default;

  Private(const Private &) = default;

  Private(const SkImageInfo &info, const void *addr, size_t row_bytes)
      : sk_pixmap(info, addr, row_bytes) {}

  ~Private() = default;

  Private &operator=(const Private &) = default;

  SkPixmap sk_pixmap;

};

}
}

#endif // WIZTK_GRAPHICS_PIXMAP_PRIVATE_HPP_
