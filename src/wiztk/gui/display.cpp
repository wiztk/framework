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

#include "display/private.hpp"

#include "wiztk/base/property.hpp"

#include "wiztk/gui/surface.hpp"

#include <iostream>

namespace wiztk {
namespace gui {

Display::Display() {
  p_ = std::make_unique<Private>();
  p_->cursors.resize(kCursorBlank, nullptr);
}

Display::~Display() = default;

void Display::Connect(const char *name) {
  if (p_->wl_display) return;

  p_->wl_display = wl_display_connect(name);
  wl_display_add_listener(p_->wl_display, &Private::kDisplayListener, this);

  if (nullptr == p_->wl_display) {
    throw std::runtime_error("FATAL! Cannot connect to Wayland compositor!");
  }
  p_->fd = wl_display_get_fd(p_->wl_display);

  p_->xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
  if (p_->xkb_context == nullptr) {
    throw std::runtime_error("FATAL! Cannot create xkb_context!");
  }

  p_->InitializeEGLDisplay();
  p_->CreateVKInstance();

  p_->wl_registry = wl_display_get_registry(p_->wl_display);
  wl_registry_add_listener(p_->wl_registry, &Private::kRegistryListener, this);

  if (wl_display_roundtrip(p_->wl_display) < 0) {
    Disconnect();
    throw std::runtime_error("Failed to process Wayland connection!");
  }

  // TODO: more operations
}

void Display::Disconnect() noexcept {
  if (nullptr == p_->wl_display) return;

  xkb_context_unref(p_->xkb_context);

  // TODO: other operations

  p_->output_manager.Clear();
  p_->input_manager.Clear();
  Surface::Clear();

  if (p_->wl_data_device_manager) {
    wl_data_device_manager_destroy(p_->wl_data_device_manager);
    p_->wl_data_device_manager = nullptr;
  }

  if (p_->wl_cursor_theme) {
    ReleaseCursors();
    wl_cursor_theme_destroy(p_->wl_cursor_theme);
    p_->wl_cursor_theme = nullptr;
  }

  if (p_->wl_shell) {
    wl_shell_destroy(p_->wl_shell);
    p_->wl_shell = nullptr;
  }

  if (p_->xdg_shell) {
    zxdg_shell_v6_destroy(p_->xdg_shell);
    p_->xdg_shell = nullptr;
  }

  if (p_->wl_shm) {
    wl_shm_destroy(p_->wl_shm);
    p_->wl_shm = nullptr;
  }

  if (p_->wl_subcompositor) {
    wl_subcompositor_destroy(p_->wl_subcompositor);
    p_->wl_subcompositor = nullptr;
  }

  if (p_->wl_compositor) {
    wl_compositor_destroy(p_->wl_compositor);
    p_->wl_compositor = nullptr;
  }

  if (p_->wl_registry) {
    wl_registry_destroy(p_->wl_registry);
    p_->wl_registry = nullptr;
  }

  p_->ReleaseVKInstance();
  p_->ReleaseEGLDisplay();

  wl_display_disconnect(p_->wl_display);
}

OutputManager *Display::GetOutputManager() const {
  return &__PROPERTY__(output_manager);
}

InputManager *Display::GetInputManager() const {
  return &__PROPERTY__(input_manager);
}

const std::set<uint32_t> &Display::GetPixelFormats() const {
  return __PROPERTY__(pixel_formats);
}

const Cursor *Display::GetCursor(CursorType cursor_type) const {
  return __PROPERTY__(cursors)[cursor_type];
}

void Display::InitializeCursors() {
  p_->cursors[kCursorBottomLeft] =
      Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "bottom_left_corner"));
  p_->cursors[kCursorBottomRight] =
      Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "bottom_right_corner"));
  p_->cursors[kCursorBottom] =
      Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "bottom_side"));
  p_->cursors[kCursorDragging] =
      Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "grabbing"));
  p_->cursors[kCursorLeftPtr] =
      Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "left_ptr"));
  p_->cursors[kCursorLeft] =
      Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "left_side"));
  p_->cursors[kCursorRight] =
      Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "right_side"));
  p_->cursors[kCursorTopLeft] =
      Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "top_left_corner"));
  p_->cursors[kCursorTopRight] =
      Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "top_right_corner"));
  p_->cursors[kCursorTop] =
      Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "top_side"));
  p_->cursors[kCursorIbeam] =
      Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "xterm"));
  p_->cursors[kCursorHand1] =
      Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "hand1"));
  p_->cursors[kCursorWatch] =
      Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "watch"));
  p_->cursors[kCursorDndMove] =
      Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "left_ptr"));
  p_->cursors[kCursorDndCopy] =
      Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "left_ptr"));
  p_->cursors[kCursorDndForbidden] =
      Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "left_ptr"));
}

void Display::ReleaseCursors() {
  for (size_t i = 0; i < p_->cursors.size(); i++) {
    delete p_->cursors[i];
    p_->cursors[i] = nullptr;
  }
}

} // namespace gui
} // namespace wiztk
