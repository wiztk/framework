/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
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

#include "abstract-rendering-api_proxy.hpp"
#include "display/private.hpp"

#include "wiztk/gui/glesv2-api.hpp"
#include "wiztk/gui/application.hpp"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <wayland-egl.h>

namespace wiztk {
namespace gui {

struct GLESV2API::Private {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);

  Private() = default;

  ~Private() = default;

  EGLSurface egl_surface = nullptr;

  struct wl_egl_window *wl_egl_window = nullptr;

};

GLESV2API::GLESV2API() {
  p_ = std::make_unique<Private>();
}

GLESV2API::~GLESV2API() {
  Destroy();
}

void GLESV2API::SetViewportSize(int width, int height) {
  wl_egl_window_resize(p_->wl_egl_window, width, height, 0, 0);
}

void GLESV2API::MakeCurrent() {
  Display *display = Application::GetInstance()->GetDisplay();

  eglMakeCurrent(Display::Private::Get(*display).egl_display,
                 p_->egl_surface,
                 p_->egl_surface,
                 Display::Private::Get(*display).egl_context);
}

void GLESV2API::SwapBuffers() {
  Display *display = Application::GetInstance()->GetDisplay();
  eglSwapBuffers(Display::Private::Get(*display).egl_display, p_->egl_surface);
}

void GLESV2API::OnSetup(Surface *surface) {
  Destroy();

  Display *display = Application::GetInstance()->GetDisplay();
  p_->wl_egl_window = wl_egl_window_create(Proxy::GetWaylandSurface(surface), 400, 300);
  p_->egl_surface = eglCreatePlatformWindowSurface(Display::Private::Get(*display).egl_display,
                                                   Display::Private::Get(*display).egl_config,
                                                   p_->wl_egl_window,
                                                   nullptr);
}

void GLESV2API::OnRelease(Surface *surface) {
  Destroy();
}

void GLESV2API::Destroy() {
  if (nullptr != p_->egl_surface) {
    _ASSERT(nullptr != p_->wl_egl_window);
    Display *display = Application::GetInstance()->GetDisplay();
    eglDestroySurface(Display::Private::Get(*display).egl_display, p_->egl_surface);
    wl_egl_window_destroy(p_->wl_egl_window);
    p_->wl_egl_window = nullptr;
    p_->egl_surface = nullptr;
  }
}

} // namespace gui
} // namespace wiztk
