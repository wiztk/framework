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

#include "abstract-egl-backend/private.hpp"

#include "wiztk/gui/application.hpp"
#include "display/proxy.hpp"

namespace wiztk {
namespace gui {

AbstractEGLBackend::AbstractEGLBackend()
    : AbstractBackend() {
  p_ = std::make_unique<Private>();

  Display *display = Application::GetInstance()->GetDisplay();
  p_->egl_display = eglGetPlatformDisplay(EGL_PLATFORM_WAYLAND_KHR,
                                          Display::Proxy::wl_display(display),
                                          nullptr);
  if (p_->egl_display == EGL_NO_DISPLAY) {
    _DEBUG("%s\n", "Fail to get egl display.");
  }
}

AbstractEGLBackend::~AbstractEGLBackend() {
  if (EGL_NO_DISPLAY != p_->egl_display)
    eglTerminate(p_->egl_display);
}

bool AbstractEGLBackend::IsValid() const {
  return EGL_NO_DISPLAY != p_->egl_display;
}

} // namespace gui
} // namespace wiztk
