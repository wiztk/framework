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

#ifndef WIZTK_GUI_ABSTRACT_EGL_BACKEND_PRIVATE_HPP_
#define WIZTK_GUI_ABSTRACT_EGL_BACKEND_PRIVATE_HPP_

#include "wiztk/gui/abstract-egl-backend.hpp"

#include <EGL/egl.h>
#include <EGL/eglext.h>

namespace wiztk {
namespace gui {

/**
 * @brief Private data used in AbstractEGLBackend.
 */
struct AbstractEGLBackend::Private {

  /**
   * @brief The native EGL display object.
   */
  EGLDisplay egl_display = EGL_NO_DISPLAY;

  /**
   * @brief Major version number.
   */
  EGLint version_major = 0;

  /**
   * @brief Minor version numberl
   */
  EGLint version_minor = 0;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_ABSTRACT_EGL_BACKEND_PRIVATE_HPP_
