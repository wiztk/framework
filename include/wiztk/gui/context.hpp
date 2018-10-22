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

#ifndef WIZTK_GUI_CONTEXT_HPP_
#define WIZTK_GUI_CONTEXT_HPP_

#include "surface.hpp"
#include "callback.hpp"

namespace wiztk {

namespace graphics {
class Canvas;
}

namespace gui {

/**
 * @ingroup gui
 * @brief The context used to draw a view
 */
class Context {

 public:

  using Canvas = graphics::Canvas;

  Context() = default;

  Context(Surface *surface, Canvas *canvas)
      : surface_(surface), canvas_(canvas) {}

  Context(const Context &other) = default;

  ~Context() = default;

  Context &operator=(const Context &other) = default;

  Surface *surface() const { return surface_; }

  void set_surface(Surface *surface) { surface_ = surface; }

  Canvas *canvas() const { return canvas_; }

  void set_canvas(Canvas *canvas) { canvas_ = canvas; }

 private:

  Surface *surface_ = nullptr;
  Canvas *canvas_ = nullptr;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_CONTEXT_HPP_
