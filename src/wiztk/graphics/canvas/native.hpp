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

#ifndef WIZTK_FRAMEWORK_NATIVE_HPP
#define WIZTK_FRAMEWORK_NATIVE_HPP

#include "private.hpp"

namespace wiztk {
namespace graphics {

class Canvas::Native {

 public:

  static inline SkCanvas* sk_canvas(const Canvas* canvas) {
    return &canvas->p_->sk_canvas;
  }

};

} // graphics
} // wiztk

#endif //WIZTK_FRAMEWORK_NATIVE_HPP
