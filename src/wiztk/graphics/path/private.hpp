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

#ifndef WIZTK_GRAPHICS_PATH_PRIVATE_HPP_
#define WIZTK_GRAPHICS_PATH_PRIVATE_HPP_

#include "wiztk/graphics/path.hpp"

#include "SkPath.h"

namespace wiztk {
namespace graphics {

using base::Point2F;
using base::RectF;

struct Path::Private {

  static const Private &Get(const Path &path) {
    return *path.p_;
  }

  Private() = default;

  Private(const Private &orig) = default;

  ~Private() = default;

  Private &operator=(const Private &) = default;

  SkPath sk_path;

};

}
}

#endif // WIZTK_GRAPHICS_PATH_PRIVATE_HPP_
