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

#ifndef WIZTK_GRAPHICS_SHADER_PRIVATE_HPP_
#define WIZTK_GRAPHICS_SHADER_PRIVATE_HPP_

#include <wiztk/graphics/shader.hpp>

#include "SkShader.h"

namespace wiztk {
namespace graphics {

/**
 * @brief Structure to encapsulate a sk_sp<SkShader> object
 */
struct Shader::Private {

  static const Private &Get(const Shader &shader) {
    return *shader.p_;
  }

  Private() = default;

  explicit Private(const sk_sp<SkShader> &shader)
      : sk_shader_sp(shader) {}

  Private(const Private &other) = default;

  Private &operator=(const Private &other) = default;

  Private &operator=(const sk_sp<SkShader> &shader) {
    sk_shader_sp = shader;
    return *this;
  }

  sk_sp<SkShader> sk_shader_sp;

};

} // namespace graphics
} // namespace wiztk

#endif  // WIZTK_GRAPHIC_INTERNAL_SHADER_PRIVATE_HPP_
