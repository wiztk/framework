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

#ifndef WIZTK_GRAPHIC_GRADIENT_SHADER_HPP_
#define WIZTK_GRAPHIC_GRADIENT_SHADER_HPP_

#include "../base/point.hpp"
#include "../base/color.hpp"

#include "shader.hpp"

namespace wiztk {
namespace graphics {

class Matrix;

/**
 * @ingroup graphics
 * @brief Gradient shader
 */
class GradientShader {

 public:

  static Shader MakeLinear(const base::Point2F points[2],
                           const uint32_t colors[],
                           const float pos[],
                           int count,
                           Shader::TileMode mode) {
    return MakeLinear(points, colors, pos, count, mode, 0, nullptr);
  }

  static Shader MakeLinear(const base::Point2F points[2],
                           const uint32_t colors[],
                           const float pos[],
                           int count,
                           Shader::TileMode mode,
                           uint32_t flags,
                           const Matrix *local_matrix);

  static Shader MakeLinear(const base::Point2F points[2],
                           const base::ColorF colors[],
                           const float pos[],
                           int count,
                           Shader::TileMode mode,
                           uint32_t flags,
                           const Matrix *local_matrix);

  static Shader MakeLinear(const base::Point2F points[2],
                           const base::ColorF colors[],
                           const float pos[],
                           int count,
                           Shader::TileMode mode) {
    return MakeLinear(points, colors, pos, count, mode, 0, nullptr);
  }

  static Shader MakeRadial(const base::Point2F &center,
                           float radius,
                           const uint32_t colors[],
                           const float pos[],
                           int count,
                           Shader::TileMode mode,
                           uint32_t flags,
                           const Matrix *local_matrix);

  static Shader MakeRadial(const base::Point2F &center,
                           float radius,
                           const uint32_t colors[],
                           const float pos[],
                           int count,
                           Shader::TileMode mode) {
    return MakeRadial(center, radius, colors, pos, count, mode, 0, nullptr);
  }

  static Shader MakeRadial(const base::Point2F &center,
                           float radius,
                           const base::ColorF colors[],
                           const float pos[],
                           int count,
                           Shader::TileMode mode,
                           uint32_t flags,
                           const Matrix *local_matrix);

  static Shader MakeRadial(const base::Point2F &center,
                           float radius,
                           const base::ColorF colors[],
                           const float pos[],
                           int count,
                           Shader::TileMode mode) {
    return MakeRadial(center, radius, colors, pos, count, mode, 0, nullptr);
  }

};

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHIC_GRADIENT_SHADER_HPP_
