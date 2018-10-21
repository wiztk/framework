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

#ifndef WIZTK_GRAPHICS_CANVAS_PRIVATE_HPP_
#define WIZTK_GRAPHICS_CANVAS_PRIVATE_HPP_

#include "wiztk/graphics/canvas.hpp"

#include "wiztk/base/deque.hpp"

#include "../surface/private.hpp"

#include "SkCanvas.h"

namespace wiztk {
namespace graphics {

/**
 * @brief The private structure used in Canvas
 */
struct Canvas::Private {

  static const Private &Get(const Canvas &canvas) {
    return *canvas.p_;
  }

  Private() {
    sk_canvas = new SkCanvas();
  }

  Private(int width, int height, const SkSurfaceProps *props = nullptr) {
    sk_canvas = new SkCanvas(width, height, props);
  }

  explicit Private(const SkBitmap &bitmap) {
    sk_canvas = new SkCanvas(bitmap);
  }

  Private(const SkBitmap &bitmap, const SkSurfaceProps &props) {
    sk_canvas = new SkCanvas(bitmap, props);
  }

  explicit Private(Surface *surface)
      : surface(surface) {
    _ASSERT(nullptr != surface);
    sk_canvas = Surface::Private::Get(*surface).sk_surface_sp->getCanvas();
  }

  ~Private() {
    if (nullptr == surface) delete sk_canvas;
  }

  SkCanvas *sk_canvas = nullptr;

  /**
   * If surface is nullptr, this canvas is constructed as standalone.
   * If surface is not nullptr, this canvas points to a an existing one on surface, it will be destroyed by the surface.
   */
  Surface *surface = nullptr;

  Point2F origin;

  size_t lock_count = 0;

  base::Deque<LockGuardNode> lock_guard_deque;

};

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHICS_INTERNAL_CANVAS_PRIVATE_HPP_
