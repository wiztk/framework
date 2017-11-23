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

#include "display_private.hpp"
#include "display_proxy.hpp"

#include "wiztk/base/macros.hpp"

#include <wiztk/gui/input.hpp>
#include <wiztk/gui/output.hpp>

namespace wiztk {
namespace gui {

const struct wl_display_listener Display::Private::kDisplayListener = {
    OnError,
    OnDeleteId
};

const struct wl_registry_listener Display::Private::kRegistryListener = {
    OnGlobal,
    OnGlobalRemove
};

const struct wl_shm_listener Display::Private::kShmListener = {
    OnFormat
};

const struct zxdg_shell_v6_listener Display::Private::kXdgShellListener = {
    OnPing
};

void Display::Private::InitializeEGLDisplay() {
  ReleaseEGLDisplay();

  EGLint count, n, size;
  EGLBoolean ret;

  EGLint config_attribs[] = {
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_RED_SIZE, 8,
      EGL_GREEN_SIZE, 8,
      EGL_BLUE_SIZE, 8,
      EGL_ALPHA_SIZE, 8,
      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
      EGL_NONE
  };
  static const EGLint context_attribs[] = {
      EGL_CONTEXT_CLIENT_VERSION, 2,
      EGL_NONE
  };

  egl_display = GetEGLDisplay(EGL_PLATFORM_WAYLAND_KHR, wl_display, NULL);
  if (!egl_display) {
    fprintf(stderr, "Cannot get EGL display!\n");
    exit(1);
  }

  ret = eglInitialize(egl_display, &egl_version_major, &egl_version_minor);
  if (ret != EGL_TRUE) {
    fprintf(stderr, "Cannot initialize EGL!\n");
    exit(1);
  }

  ret = eglBindAPI(EGL_OPENGL_ES_API);
  if (ret != EGL_TRUE) {
    fprintf(stderr, "Cannot bind EGL API!\n");
    exit(1);
  }

  eglGetConfigs(egl_display, NULL, 0, &count);

  EGLConfig *configs = (EGLConfig *) calloc((size_t) count, sizeof(EGLConfig));
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

  egl_context = eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT, context_attribs);
  if (!egl_context) {
    fprintf(stderr, "Cannot create EGL context!\n");
    exit(1);
  }

  static const struct {
    const char *extension, *entrypoint;
  } swap_damage_ext_to_entrypoint[] = {
      {
          .extension = "EGL_EXT_swap_buffers_with_damage",
          .entrypoint = "eglSwapBuffersWithDamageEXT",
      },
      {
          .extension = "EGL_KHR_swap_buffers_with_damage",
          .entrypoint = "eglSwapBuffersWithDamageKHR",
      },
  };
  const char *extensions;

  extensions = eglQueryString(egl_display, EGL_EXTENSIONS);
  if (extensions &&
      CheckEGLExtension(extensions, "EGL_EXT_buffer_age")) {
//    int len = (int) ARRAY_LENGTH(swap_damage_ext_to_entrypoint);
    int i = 0;
    for (i = 0; i < 2; i++) {
      if (CheckEGLExtension(extensions,
                            swap_damage_ext_to_entrypoint[i].extension)) {
        /* The EXTPROC is identical to the KHR one */
        Proxy::kSwapBuffersWithDamageAPI =
            (PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC)
                eglGetProcAddress(swap_damage_ext_to_entrypoint[i].entrypoint);
        break;
      }
    }
    if (Proxy::kSwapBuffersWithDamageAPI)
      printf("has EGL_EXT_buffer_age and %s\n", swap_damage_ext_to_entrypoint[i].extension);
  }
}

void Display::Private::ReleaseEGLDisplay() {
  if (egl_display) {
    eglMakeCurrent(egl_display, (::EGLSurface) 0, (::EGLSurface) 0, (::EGLContext) 0);
    eglTerminate(egl_display);
    eglReleaseThread();

    egl_display = nullptr;
    egl_context = nullptr;
    egl_config = nullptr;
    egl_version_major = 0;
    egl_version_minor = 0;
  }
}

void Display::Private::CreateVKInstance() {
  vk::InstanceCreateInfo create_info = {};
  vk_instance = vk::createInstance(create_info);

  if (!vk_instance) {
    throw std::runtime_error("Error! Fail to create Vulkan instance!");
  }
}

void Display::Private::ReleaseVKInstance() {
  vk_instance.destroy();
}

void Display::Private::MakeSwapBufferNonBlock() const {
  EGLint a = EGL_MIN_SWAP_INTERVAL;
  EGLint b = EGL_MAX_SWAP_INTERVAL;

  if (!eglGetConfigAttrib(egl_display, egl_config, a, &a) ||
      !eglGetConfigAttrib(egl_display, egl_config, b, &b)) {
    fprintf(stderr, "warning: swap interval range unknown\n");
  } else if (a > 0) {
    fprintf(stderr, "warning: minimum swap interval is %d, "
        "while 0 is required to not deadlock on resize.\n", a);
  }

  /*
   * We rely on the Wayland compositor to sync to vblank anyway.
   * We just need to be able to call eglSwapBuffers() without the
   * risk of waiting for a frame callback in it.
   */
  if (!eglSwapInterval(egl_display, 0)) {
    fprintf(stderr, "error: eglSwapInterval() failed.\n");
  }
}

void Display::Private::OnFormat(void *data, struct wl_shm *shm, uint32_t format) {
  auto *_this = static_cast<Display *>(data);
  const char *text = nullptr;

  _this->p_->pixel_formats.insert(format);

  switch (format) {
    case WL_SHM_FORMAT_ARGB8888: {
      text = "ARGB8888";
      break;
    }
    case WL_SHM_FORMAT_XRGB8888: {
      text = "XRGB8888";
      break;
    }
    default: {
      text = "other format";
      break;
    }
  }

  fprintf(stderr, "Possible shmem format %s - %d\n", text, format);
}

void Display::Private::OnError(void *data,
                               struct wl_display *wl_display,
                               void *object_id,
                               uint32_t code,
                               const char *message) {
  auto *_this = static_cast<Display *>(data);
  const char *object_name = "Unknown object";

  if (_this->p_->wl_display == object_id) {
    object_name = "wl_display";
  } else if (_this->p_->wl_compositor == object_id) {
    object_name = "wl_compositor";
  } else if (_this->p_->wl_registry == object_id) {
    object_name = "wl_registry";
  } else if (_this->p_->wl_subcompositor == object_id) {
    object_name = "wl_subcompositor";
  } else if (_this->p_->wl_shm == object_id) {
    object_name = "wl_shm";
  } else if (_this->p_->wl_shell == object_id) {
    object_name = "wl_shell";
  } else if (_this->p_->xdg_shell == object_id) {
    object_name = "xdg_shell";
  }

  fprintf(stderr, "Error from %s: %s (%d)\n", object_name, message, code);
}

void Display::Private::OnDeleteId(void *data, struct wl_display *wl_display, uint32_t id) {
//  Display *_this = static_cast<Display *>(data);
  _DEBUG("Delete ID: %ud\n", id);
}

void Display::Private::OnGlobal(void *data,
                                struct wl_registry *registry,
                                uint32_t id,
                                const char *interface,
                                uint32_t version) {
  auto *_this = static_cast<Display *>(data);

  auto *global = new Global;
  global->id = id;
  global->interface = interface;
  global->version = version;
  _this->p_->globals.push_back(global);

  if (strcmp(interface, wl_compositor_interface.name) == 0) {
    _this->p_->wl_compositor =
        static_cast<struct wl_compositor *>(wl_registry_bind(_this->p_->wl_registry,
                                                             id,
                                                             &wl_compositor_interface,
                                                             version));
  } else if (strcmp(interface, wl_subcompositor_interface.name) == 0) {
    _this->p_->wl_subcompositor =
        static_cast<struct wl_subcompositor *>(wl_registry_bind(_this->p_->wl_registry,
                                                                id,
                                                                &wl_subcompositor_interface,
                                                                version));
  } else if (strcmp(interface, wl_shm_interface.name) == 0) {
    _this->p_->wl_shm =
        static_cast<struct wl_shm *>(wl_registry_bind(_this->p_->wl_registry,
                                                      id,
                                                      &wl_shm_interface,
                                                      version));
    wl_shm_add_listener(_this->p_->wl_shm, &Private::kShmListener, _this);
    _ASSERT(nullptr == _this->p_->wl_cursor_theme);
    _this->p_->wl_cursor_theme = wl_cursor_theme_load(NULL, 24, _this->p_->wl_shm);

    _this->InitializeCursors();
  } else if (strcmp(interface, wl_output_interface.name) == 0) {
    auto *output = new Output(id, version);
    _this->AddOutput(output);
  } else if (strcmp(interface, zxdg_shell_v6_interface.name) == 0) {
    _this->p_->xdg_shell =
        static_cast<struct zxdg_shell_v6 *>(wl_registry_bind(_this->p_->wl_registry,
                                                             id,
                                                             &zxdg_shell_v6_interface,
                                                             version));
    zxdg_shell_v6_add_listener(_this->p_->xdg_shell, &Private::kXdgShellListener, _this);
  } else if (strcmp(interface, wl_shell_interface.name) == 0) {
    _this->p_->wl_shell =
        static_cast<struct wl_shell *>(wl_registry_bind(_this->p_->wl_registry,
                                                        id, &wl_shell_interface,
                                                        version));
  } else if (strcmp(interface, wl_seat_interface.name) == 0) {
    auto *input = new Input(id, version);
    _this->AddInput(input);
  } else if (strcmp(interface, wl_data_device_manager_interface.name) == 0) {
    _this->p_->wl_data_device_manager =
        static_cast<struct wl_data_device_manager *>(wl_registry_bind(_this->p_->wl_registry,
                                                                      id,
                                                                      &wl_data_device_manager_interface,
                                                                      version));
  }
}

void Display::Private::OnGlobalRemove(void *data,
                                      struct wl_registry *registry,
                                      uint32_t name) {
  auto *_this = static_cast<Display *>(data);

  for (auto it = _this->p_->globals.begin(); it != _this->p_->globals.end();) {
    if ((*it)->id != name) {
      it++;
      continue;
    }

    if ((*it)->interface == "wl_output") {
      _this->DestroyOutput(name);
    }

    _this->unregister_.Emit(**it);

    delete (*it);
    it = _this->p_->globals.erase(it);
  }
}

void Display::Private::OnPing(void *data, struct zxdg_shell_v6 *zxdg_shell_v6, uint32_t serial) {
  auto *_this = static_cast<Display *>(data);
  zxdg_shell_v6_pong(_this->p_->xdg_shell, serial);
}

EGLDisplay Display::Private::GetEGLDisplay(EGLenum platform, void *native_display, const EGLint *attrib_list) {
  static PFNEGLGETPLATFORMDISPLAYEXTPROC kEGLGetPlatformDisplayHandle = NULL;

  if (!kEGLGetPlatformDisplayHandle) {
    kEGLGetPlatformDisplayHandle = (PFNEGLGETPLATFORMDISPLAYEXTPROC)
        GetEGLProcAddress("eglGetPlatformDisplayEXT");
  }

  if (kEGLGetPlatformDisplayHandle)
    return kEGLGetPlatformDisplayHandle(platform,
                                        native_display, attrib_list);

  return eglGetDisplay((EGLNativeDisplayType) native_display);
}

void *Display::Private::GetEGLProcAddress(const char *address) {
  const char *extensions = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);

  if (extensions &&
      (CheckEGLExtension(extensions, "EGL_EXT_platform_wayland") ||
          CheckEGLExtension(extensions, "EGL_KHR_platform_wayland"))) {
    return (void *) eglGetProcAddress(address);
  }

  return nullptr;
}

bool Display::Private::CheckEGLExtension(const char *extensions, const char *extension) {
  size_t extlen = strlen(extension);
  const char *end = extensions + strlen(extensions);

  while (extensions < end) {
    size_t n = 0;

    /* Skip whitespaces, if any */
    if (*extensions == ' ') {
      extensions++;
      continue;
    }

    n = strcspn(extensions, " ");

    /* Compare strings */
    if (n == extlen && strncmp(extension, extensions, n) == 0)
      return true; /* Found */

    extensions += n;
  }

  /* Not found */
  return false;
}

} // namespace gui
} // namespace wiztk
