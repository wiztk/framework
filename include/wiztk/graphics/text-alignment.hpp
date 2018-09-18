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

#ifndef WIZTK_GRAPHICS_TEXT_ALIGNMENT_HPP_
#define WIZTK_GRAPHICS_TEXT_ALIGNMENT_HPP_

#include "wiztk/base/macros.hpp"

namespace wiztk {
namespace graphics {

class WIZTK_EXPORT TextAlignment {

 public:

  enum Horizontal {
    kLeft,
    kCenter,
    kRight
  };

  static constexpr int kHorizontalMask = 0x0F;

  enum Vertical {
    kTop = 0x0,
    kMiddle = (0x1 << 4),
    kBottom = (0x2 << 4),
    kBaseline = (0x3 << 4)
  };

  static constexpr int kVerticalMask = 0xF0;

};

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHICS_TEXT_ALIGNMENT_HPP_
