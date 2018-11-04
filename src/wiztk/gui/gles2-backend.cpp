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

#include "gles2-backend/private.hpp"
#include "abstract-egl-backend/private.hpp"

#include "wiztk/gui/application.hpp"
#include "surface/private.hpp"

#include "display/private.hpp"

namespace wiztk {
namespace gui {

GLES2Backend::GLES2Backend(Surface *surface)
    : AbstractEGLBackend(surface) {
  p_ = std::make_unique<Private>();

  EGLBoolean ret = eglBindAPI(EGL_OPENGL_ES_API);
  if (EGL_TRUE != ret) {
    _DEBUG("%s\n", "Cannot bind OpenGL ES API!");
    throw std::runtime_error("Error! Fail to bind OpenGL ES API!");
  }

  EGLint count, n, size;

  EGLint config_attribs[] = {
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT, // TODO: support PIXMAP/PBUFFER
      EGL_RED_SIZE, 8,
      EGL_GREEN_SIZE, 8,
      EGL_BLUE_SIZE, 8,
      EGL_ALPHA_SIZE, 8,
      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
      EGL_NONE
  };
  const EGLint context_attribs[] = {
      EGL_CONTEXT_CLIENT_VERSION, 2,
      EGL_NONE
  };

  EGLDisplay egl_display = AbstractEGLBackend::Private::Get(*this).egl_display;
  EGLConfig egl_config = nullptr;

  eglGetConfigs(egl_display, nullptr, 0, &count);
  auto *configs = (EGLConfig *) calloc((size_t) count, sizeof(EGLConfig));
  eglChooseConfig(egl_display, config_attribs, configs, count, &n);
  for (int i = 0; i < n; i++) {
    eglGetConfigAttrib(egl_display, configs[i], EGL_BUFFER_SIZE, &size);
    if (32 == size) {
      // TODO: config buffer size
      egl_config = configs[i];
      break;
    }
  }
  free(configs);
  _ASSERT(egl_config);

  p_->egl_context = eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT, context_attribs);
  if (nullptr == p_->egl_context) {
    _DEBUG("%s\n", "Fail to create EGL context!");
    throw std::runtime_error("Error! Cannot create EGL Context!");
  }

//  Display *display = Application::GetInstance()->GetDisplay();
//  p_->wl_egl_window = wl_egl_window_create(Proxy::GetWaylandSurface(surface), 400, 300);
//  p_->egl_surface = eglCreatePlatformWindowSurface(AbstractEGLBackend::p_->egl_display,
//                                                   AbstractEGLBackend::p_->egl_display,
//                                                   p_->wl_egl_window,
//                                                   nullptr);
}

GLES2Backend::~GLES2Backend() {
//  _ASSERT(nullptr != p_->egl_surface);
//  _ASSERT(nullptr != p_->wl_egl_window);
//  Display *display = Application::GetInstance()->GetDisplay();
//  eglDestroySurface(Display::Proxy::egl_display(display), p_->egl_surface);
//  wl_egl_window_destroy(p_->wl_egl_window);
  if (nullptr != p_->egl_context) {
    EGLDisplay egl_display = AbstractEGLBackend::Private::Get(*this).egl_display;
    eglDestroyContext(egl_display, p_->egl_context);
  }
}

void GLES2Backend::SetViewportSize(int width, int height) {

}

void GLES2Backend::Render(Surface *surface) {

}

} // namespace gui
} // namespace wiztk
