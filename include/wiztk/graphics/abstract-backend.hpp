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

#ifndef WIZTK_GRAPHICS_ABSTRACT_BACKEND_HPP_
#define WIZTK_GRAPHICS_ABSTRACT_BACKEND_HPP_

namespace wiztk {
namespace graphics {

// Forward declarations:
class AbstractSurface;

/**
 * @ingroup graphics
 * @brief The graphics backend used for a surface.
 *
 * A backend object represents the different GL APIs including:
 * - OpenGL ES 3.x
 * - OpenGL 4.x
 * - Vulkan
 */
class AbstractBackend {

 public:

  AbstractBackend();

  virtual ~AbstractBackend();

  void Setup(AbstractSurface *surface);

  void Release(AbstractSurface *surface);

 protected:

  virtual void OnSetup(AbstractSurface *surface) = 0;

  virtual void OnRelease(AbstractSurface *surface) = 0;

};

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHICS_ABSTRACT_BACKEND_HPP_
