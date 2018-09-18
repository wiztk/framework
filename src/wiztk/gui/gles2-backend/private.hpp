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

#ifndef WIZTK_FRAMEWORK_GLES2_BACKEND_PRIVATE_HPP
#define WIZTK_FRAMEWORK_GLES2_BACKEND_PRIVATE_HPP

#include "wiztk/gui/gles2-backend.hpp"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <wayland-egl.h>

namespace wiztk {
namespace gui {

/**
* @brief Private data in GLES2Backend.
*/
struct GLES2Backend::Private {

  EGLContext egl_context = nullptr;

  EGLSurface egl_surface = nullptr;

  struct wl_egl_window *wl_egl_window = nullptr;

};

}
}

#endif //WIZTK_FRAMEWORK_GLES2_BACKEND_PRIVATE_HPP
