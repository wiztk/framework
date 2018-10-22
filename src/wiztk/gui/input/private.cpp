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

#include "private.hpp"

#include <wiztk/base/macros.hpp>

#include <unistd.h>
#include <sys/mman.h>

#include <iostream>

namespace wiztk {
namespace gui {

const struct wl_seat_listener Input::Private::kSeatListener = {
    OnSeatCapabilities,
    OnSeatName
};

const struct wl_pointer_listener Input::Private::kPointerListener = {
    OnPointerEnter,
    OnPointerLeave,
    OnPointerMotion,
    OnPointerButton,
    OnPointerAxis,
    OnPointerFrame,
    OnPointerAxisSource,
    OnPointerAxisStop,
    OnPointerAxisDiscrete
};

const struct wl_keyboard_listener Input::Private::kKeyboardListener = {
    OnKeyboardKeymap,
    OnKeyboardEnter,
    OnKeyboardLeave,
    OnKeyboardKey,
    OnKeyboardModifiers,
    OnKeyboardRepeatInfo
};

const struct wl_touch_listener Input::Private::kTouchListener = {
    OnTouchDown,
    OnTouchUp,
    OnTouchMotion,
    OnTouchFrame,
    OnTouchCancel
};

void Input::Private::OnSeatCapabilities(void *data, struct wl_seat *wl_seat, uint32_t capabilities) {
  auto *_this = static_cast<Input *>(data);

  if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD) {
    _ASSERT(nullptr == _this->p_->wl_keyboard);

    _this->p_->wl_keyboard = wl_seat_get_keyboard(wl_seat);
    wl_keyboard_add_listener(_this->p_->wl_keyboard, &kKeyboardListener, _this);

    _ASSERT(nullptr == _this->p_->key_event);
    _this->p_->key_event = new KeyEvent(_this);
  }
  if (capabilities & WL_SEAT_CAPABILITY_POINTER) {
    _ASSERT(nullptr == _this->p_->wl_pointer);
    _this->p_->wl_pointer = wl_seat_get_pointer(wl_seat);
    wl_pointer_add_listener(_this->p_->wl_pointer, &kPointerListener, _this);

    _ASSERT(nullptr == _this->p_->mouse_event);
    _this->p_->mouse_event = new MouseEvent(_this);
  }
  if (capabilities & WL_SEAT_CAPABILITY_TOUCH) {
    _ASSERT(_this->p_->wl_touch == nullptr);
    _this->p_->wl_touch = wl_seat_get_touch(wl_seat);
    wl_touch_add_listener(_this->p_->wl_touch, &kTouchListener, _this);
    _ASSERT(_this->p_->touch_event == nullptr);
    _this->p_->touch_event = new TouchEvent(_this);
  }
}

void Input::Private::OnSeatName(void *data, struct wl_seat *wl_seat, const char *name) {
  auto *_this = static_cast<Input *>(data);
  _this->p_->name = name;
}

void Input::Private::OnPointerEnter(void *data,
                                    struct wl_pointer *wl_pointer,
                                    uint32_t serial,
                                    struct wl_surface *wl_surface,
                                    wl_fixed_t surface_x,
                                    wl_fixed_t surface_y) {
  auto *_this = static_cast<Input *>(data);

  _this->p_->mouse_event->p_->serial = serial;
  _this->p_->mouse_event->p_->surface_xy.x = wl_fixed_to_double(surface_x);
  _this->p_->mouse_event->p_->surface_xy.y = wl_fixed_to_double(surface_y);

  _this->p_->mouse_event->p_->surface =
      static_cast<Surface *>(wl_surface_get_user_data(wl_surface));

  _this->p_->mouse_event->response_ = InputEvent::kUnknown;
  _this->p_->mouse_event->p_->surface->GetEventHandler()->OnMouseEnter(_this->p_->mouse_event);
}

void Input::Private::OnPointerLeave(void *data,
                                    struct wl_pointer *wl_pointer,
                                    uint32_t serial,
                                    struct wl_surface *wl_surface) {
  auto *_this = static_cast<Input *>(data);

  _this->p_->mouse_event->p_->serial = serial;
  _this->p_->mouse_event->p_->surface = static_cast<Surface *>(wl_surface_get_user_data(wl_surface));

  _this->p_->mouse_event->response_ = InputEvent::kUnknown;
  _this->p_->mouse_event->p_->surface->GetEventHandler()->OnMouseLeave();
}

void Input::Private::OnPointerMotion(void *data,
                                     struct wl_pointer *wl_pointer,
                                     uint32_t time,
                                     wl_fixed_t surface_x,
                                     wl_fixed_t surface_y) {
  auto *_this = static_cast<Input *>(data);

  _this->p_->mouse_event->p_->time = time;
  _this->p_->mouse_event->p_->surface_xy.x = wl_fixed_to_double(surface_x);
  _this->p_->mouse_event->p_->surface_xy.y = wl_fixed_to_double(surface_y);

  if (nullptr == _this->p_->mouse_event->p_->surface) return;

  _this->p_->mouse_event->response_ = InputEvent::kUnknown;
  _this->p_->mouse_event->p_->surface->GetEventHandler()->OnMouseMove(_this->p_->mouse_event);
}

void Input::Private::OnPointerButton(void *data,
                                     struct wl_pointer *wl_pointer,
                                     uint32_t serial,
                                     uint32_t time,
                                     uint32_t button,
                                     uint32_t state) {
  auto *_this = static_cast<Input *>(data);

  _this->p_->mouse_event->response_ = InputEvent::kUnknown;
  _this->p_->mouse_event->p_->serial = serial;
  _this->p_->mouse_event->p_->time = time;
  _this->p_->mouse_event->p_->button = button;
  _this->p_->mouse_event->p_->state = state;

  if (nullptr == _this->p_->mouse_event->p_->surface) return;

  _this->p_->mouse_event->response_ = InputEvent::kUnknown;

  if (state == WL_POINTER_BUTTON_STATE_PRESSED) {
    _this->p_->mouse_event->p_->surface->GetEventHandler()->OnMouseDown(_this->p_->mouse_event);
  } else if (state == WL_POINTER_BUTTON_STATE_RELEASED) {
    _this->p_->mouse_event->p_->surface->GetEventHandler()->OnMouseUp(_this->p_->mouse_event);
  }
}

void Input::Private::OnPointerAxis(void *data,
                                   struct wl_pointer *wl_pointer,
                                   uint32_t time,
                                   uint32_t axis,
                                   wl_fixed_t value) {

}

void Input::Private::OnPointerFrame(void *data, struct wl_pointer *wl_pointer) {

}

void Input::Private::OnPointerAxisSource(void *data, struct wl_pointer *wl_pointer, uint32_t axis_source) {

}

void Input::Private::OnPointerAxisStop(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis) {

}

void Input::Private::OnPointerAxisDiscrete(void *data, struct wl_pointer *wl_pointer, uint32_t axis, int32_t discrete) {

}

void Input::Private::OnKeyboardKeymap(void *data,
                                      struct wl_keyboard *wl_keyboard,
                                      uint32_t format,
                                      int32_t fd,
                                      uint32_t size) {
  auto *_this = static_cast<Input *>(data);

  if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) {
    close(fd);
    return;
  }

  char *string = (char *) mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
  if (string == MAP_FAILED) {
    close(fd);
    return;
  }

  try {
    _this->p_->keymap.Setup(string, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
  } catch (const std::runtime_error &e) {
    close(fd);
    std::cerr << e.what() << std::endl;
    return;
  }

  munmap(string, size);
  close(fd);

  try {
    _this->p_->keyboard_state.Setup(_this->p_->keymap);
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return;
  }

  // TODO: read locale
}

void Input::Private::OnKeyboardEnter(void *data,
                                     struct wl_keyboard *wl_keyboard,
                                     uint32_t serial,
                                     struct wl_surface *wl_surface,
                                     struct wl_array *keys) {
  auto *_this = static_cast<Input *>(data);

  _this->p_->key_event->serial_ = serial;
//  keyboard_event_.surface_ = static_cast<Surface*>(wl_surface_get_user_data((wl_surface)));
  // TODO: use the keys
//  void *p = nullptr;
//  int count = 0;
//  wl_array_for_each(p, keys) {
//    count++;
//  }
}

void Input::Private::OnKeyboardLeave(void *data,
                                     struct wl_keyboard *wl_keyboard,
                                     uint32_t serial,
                                     struct wl_surface *wl_surface) {
  auto *_this = static_cast<Input *>(data);

  _this->p_->key_event->serial_ = serial;

}

void Input::Private::OnKeyboardKey(void *data,
                                   struct wl_keyboard *wl_keyboard,
                                   uint32_t serial,
                                   uint32_t time,
                                   uint32_t key,
                                   uint32_t state) {
  auto *_this = static_cast<Input *>(data);

  _this->p_->key_event->serial_ = serial;
  _this->p_->key_event->time_ = serial;
  _this->p_->key_event->key_ = key;
  _this->p_->key_event->state_ = state;

  if (state == WL_KEYBOARD_KEY_STATE_PRESSED) {
    // OnKeyDown;
  } else if (state == WL_KEYBOARD_KEY_STATE_RELEASED) {
    // OnKeyUp;
  }
}

void Input::Private::OnKeyboardModifiers(void *data,
                                         struct wl_keyboard *wl_keyboard,
                                         uint32_t serial,
                                         uint32_t mods_depressed,
                                         uint32_t mods_latched,
                                         uint32_t mods_locked,
                                         uint32_t group) {
  auto *_this = static_cast<Input *>(data);

  _this->p_->key_event->serial_ = serial;
  _this->p_->key_event->mods_depressed_ = mods_depressed;
  _this->p_->key_event->mods_latched_ = mods_latched;
  _this->p_->key_event->mods_locked_ = mods_locked;
  _this->p_->key_event->group_ = group;
}

void Input::Private::OnKeyboardRepeatInfo(void *data, struct wl_keyboard *wl_keyboard, int32_t rate, int32_t delay) {

}

void Input::Private::OnTouchDown(void *data,
                                 struct wl_touch *wl_touch,
                                 uint32_t serial,
                                 uint32_t time,
                                 struct wl_surface *surface,
                                 int32_t id,
                                 wl_fixed_t x,
                                 wl_fixed_t y) {

}

void Input::Private::OnTouchUp(void *data, struct wl_touch *wl_touch, uint32_t serial, uint32_t time, int32_t id) {

}

void Input::Private::OnTouchMotion(void *data,
                                   struct wl_touch *wl_touch,
                                   uint32_t time,
                                   int32_t id,
                                   wl_fixed_t x,
                                   wl_fixed_t y) {

}

void Input::Private::OnTouchFrame(void *data, struct wl_touch *wl_touch) {

}

void Input::Private::OnTouchCancel(void *data, struct wl_touch *wl_touch) {

}

} // namespace gui
} // namespace wiztk
