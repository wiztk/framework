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

#include "wiztk/graphics/abstract-backend.hpp"

namespace wiztk {
namespace graphics {

AbstractBackend::AbstractBackend() = default;

AbstractBackend::~AbstractBackend() = default;

void AbstractBackend::Setup(wiztk::graphics::AbstractSurface *surface) {
  OnSetup(surface);
}

void AbstractBackend::Release(wiztk::graphics::AbstractSurface *surface) {
  OnRelease(surface);
}

} // namespace graphics
} // namespace wiztk
