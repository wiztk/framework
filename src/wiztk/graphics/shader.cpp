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

#include "shader/private.hpp"

#include "wiztk/base/macros.hpp"

namespace wiztk {
namespace graphics {

Shader::Shader() {
  p_ = std::make_unique<Private>();
}

Shader::Shader(Private *p)
    : p_(p) {
  _ASSERT(p_);
}

Shader::Shader(const Shader &other) {
  p_ = std::make_unique<Private>(other.p_->sk_shader_sp);
}

Shader::~Shader() = default;

Shader &Shader::operator=(const Shader &other) {
  *p_ = *other.p_;
  return *this;
}

Shader::operator bool() const {
  return p_->sk_shader_sp.get() != nullptr;
}

} // namespace graphics
} // namespace wiztk
