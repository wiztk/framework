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

#ifndef WIZTK_GRAPHICS_IMAGE_INFO_PRIVATE_HPP_
#define WIZTK_GRAPHICS_IMAGE_INFO_PRIVATE_HPP_

#include "wiztk/graphics/image-info.hpp"

#include "SkImageInfo.h"

namespace wiztk {
namespace graphics {

/**
 * @brief Private data for ImageInfo
 */
struct ImageInfo::Private {

  static const Private &Get(const ImageInfo &info) {
    return *info.p_;
  }

  Private() = default;

  Private(const Private &) = default;

  Private(Private &&) = default;

  ~Private() = default;

  Private &operator=(const Private &) = default;

  Private &operator=(Private &&) = default;

  SkImageInfo sk_image_info;

};

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHIC_INTERNAL_IMAGE_INFO_PRIVATE_HPP_
