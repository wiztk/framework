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

#ifndef WIZTK_GRAPHICS_ABSTRACT_SURFACE_PRIVATE_HPP_
#define WIZTK_GRAPHICS_ABSTRACT_SURFACE_PRIVATE_HPP_

#include "wiztk/graphics/abstract-surface.hpp"

#include "wiztk/base/property.hpp"

namespace wiztk {
namespace graphics {

/**
 * @brief Private properties used in AbstractSurface
 */
struct AbstractSurface::Private : public base::Property<AbstractSurface> {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);
  Private() = delete;

  static const Private &Get(const AbstractSurface &surface) {
    return *surface.p_;
  }

  explicit Private(AbstractSurface *surface)
      : base::Property<AbstractSurface>(surface) {}

  ~Private() final = default;

  Size size;

  Margin margin;

  bool margin_locked = false;

  bool size_locked = false;

};

} // namespace graphics
} // namesapce wiztk

#endif // WIZTK_GRAPHICS_ABSTRACT_SURFACE_PRIVATE_HPP_
