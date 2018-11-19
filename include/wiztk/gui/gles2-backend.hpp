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

#ifndef WIZTK_GUI_GLES2_BACKEND_HPP_
#define WIZTK_GUI_GLES2_BACKEND_HPP_

#include "wiztk/gui/abstract-egl-backend.hpp"

#include <memory>

namespace wiztk {
namespace gui {

/**
 * @ingroup gui
 * @brief OpenGL ES V2 Backend.
 */
class WIZTK_EXPORT GLES2Backend : public AbstractEGLBackend {

 public:

  class Allocator : public AbstractRenderingBackend::Allocator {

   public:

    Allocator() = default;
    Allocator(const Allocator &) = default;
    Allocator(Allocator &&) = default;
    Allocator &operator=(const Allocator &) = default;
    Allocator &operator=(Allocator &&) = default;
    ~Allocator() override = default;

    AbstractRenderingBackend *operator()() const override;

  };

  GLES2Backend();

  ~GLES2Backend() override;

  void SetViewportSize(int width, int height) final;

  void Render(Surface *surface) final;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_GLES2_BACKEND_HPP_
