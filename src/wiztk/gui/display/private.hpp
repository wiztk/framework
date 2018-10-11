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

#ifndef WIZTK_GUI_DISPLAY_PRIVATE_HPP_
#define WIZTK_GUI_DISPLAY_PRIVATE_HPP_

#include "wiztk/gui/display.hpp"

#include "wiztk/gui/queued-task.hpp"
#include "wiztk/gui/abstract-epoll-task.hpp"
#include "wiztk/gui/input-manager.hpp"
#include "wiztk/gui/output-manager.hpp"
#include "wiztk/gui/cursor.hpp"

#include <list>

#include <vulkan/vulkan.hpp>

#include <xdg-shell-unstable-v6-client-protocol.h>

#include <xkbcommon/xkbcommon.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

namespace wiztk {
namespace gui {

/**
 * @brief The private data structure used in Display
 */
struct Display::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  static const Private &Get(const Display &display) {
    return *display.p_;
  }

  Private() = default;

  ~Private() = default;

  void InitializeEGLDisplay();

  void ReleaseEGLDisplay();

  void CreateVKInstance();

  void ReleaseVKInstance();

  void MakeSwapBufferNonBlock() const;

  struct wl_display *wl_display = nullptr;
  struct wl_registry *wl_registry = nullptr;
  struct wl_compositor *wl_compositor = nullptr;
  struct wl_subcompositor *wl_subcompositor = nullptr;
  struct wl_shm *wl_shm = nullptr;
  struct wl_shell *wl_shell = nullptr;
  struct zxdg_shell_v6 *xdg_shell = nullptr;
  struct wl_cursor_theme *wl_cursor_theme = nullptr;
  struct wl_data_device_manager *wl_data_device_manager = nullptr;

  EGLDisplay egl_display = nullptr;
  EGLContext egl_context = nullptr;
  EGLConfig egl_config = nullptr;

  EGLint egl_version_major = 0;  /**< The major version */
  EGLint egl_version_minor = 0;  /**< The minor version */

  vk::Instance vk_instance;

  struct xkb_context *xkb_context = nullptr;

  int fd = 0;

  InputManager input_manager;
  OutputManager output_manager;

  std::list<Global *> globals;
  std::set<uint32_t> pixel_formats;

  std::vector<Cursor *> cursors;

  static void OnFormat(void *data, struct wl_shm *shm, uint32_t format);

  static void OnError(void *data,
                      struct wl_display *wl_display,
                      void *object_id,
                      uint32_t code,
                      const char *message);

  static void OnDeleteId(void *data,
                         struct wl_display *wl_display,
                         uint32_t id);

  static void OnGlobal(void *data,
                       struct wl_registry *registry,
                       uint32_t id,
                       const char *interface,
                       uint32_t version);

  static void OnGlobalRemove(void *data,
                             struct wl_registry *registry,
                             uint32_t name);

  static void OnPing(void *data,
                     struct zxdg_shell_v6 *zxdg_shell_v6,
                     uint32_t serial);

  static EGLDisplay GetEGLDisplay(EGLenum platform, void *native_display, const EGLint *attrib_list);

  static void *GetEGLProcAddress(const char *address);

  static bool CheckEGLExtension(const char *extensions, const char *extension);

  static const struct wl_display_listener kDisplayListener;

  static const struct wl_registry_listener kRegistryListener;

  static const struct wl_shm_listener kShmListener;

  static const struct zxdg_shell_v6_listener kXdgShellListener;

  static PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC kSwapBuffersWithDamageAPI;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_DISPLAY_PRIVATE_HPP_
