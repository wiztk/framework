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

#ifndef WIZTK_GUI_INTERNAL_INPUT_PRIVATE_HPP_
#define WIZTK_GUI_INTERNAL_INPUT_PRIVATE_HPP_

#include "wiztk/gui/input.hpp"

#include "wiztk/base/counted-deque.hpp"

#include <wiztk/gui/view-surface.hpp>
#include <wiztk/gui/key-event.hpp>
#include <wiztk/gui/touch-event.hpp>

#include "mouse-event/private.hpp"
#include "keymap.hpp"
#include "keyboard_state.hpp"

namespace wiztk {
namespace gui {

/**
 * @brief Private data used in Input.
 */
struct WIZTK_NO_EXPORT Input::Private : public base::CountedDequeNodeBase {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);
  Private() = delete;

  explicit Private(Input *input)
      : proprietor(input) {}

  ~Private() final {
    keyboard_state.Destroy();
    keymap.Destroy();

    delete touch_event;
    delete mouse_event;
    delete key_event;

    if (wl_touch)
      wl_touch_destroy(wl_touch);

    if (wl_pointer)
      wl_pointer_destroy(wl_pointer);

    if (wl_keyboard)
      wl_keyboard_destroy(wl_keyboard);

    if (wl_seat)
      wl_seat_destroy(wl_seat);
  }

  Input *proprietor = nullptr;

  struct wl_seat *wl_seat = nullptr;
  struct wl_keyboard *wl_keyboard = nullptr;
  struct wl_pointer *wl_pointer = nullptr;
  struct wl_touch *wl_touch = nullptr;

  KeyEvent *key_event = nullptr;
  MouseEvent *mouse_event = nullptr;
  TouchEvent *touch_event = nullptr;

  std::string name;

  Keymap keymap;
  KeyboardState keyboard_state;

  uint32_t id = 0;
  uint32_t version = 0;

  // seat:

  static void OnSeatCapabilities(void *data, struct wl_seat *wl_seat, uint32_t capabilities);

  static void OnSeatName(void *data, struct wl_seat *wl_seat, const char *name);

  // pointer:

  static void OnPointerEnter(void *data,
                             struct wl_pointer *wl_pointer,
                             uint32_t serial,
                             struct wl_surface *wl_surface,
                             wl_fixed_t surface_x,
                             wl_fixed_t surface_y);

  static void OnPointerLeave(void *data,
                             struct wl_pointer *wl_pointer,
                             uint32_t serial,
                             struct wl_surface *wl_surface);

  static void OnPointerMotion(void *data,
                              struct wl_pointer *wl_pointer,
                              uint32_t time,
                              wl_fixed_t surface_x,
                              wl_fixed_t surface_y);

  static void OnPointerButton(void *data,
                              struct wl_pointer *wl_pointer,
                              uint32_t serial,
                              uint32_t time,
                              uint32_t button,
                              uint32_t state);

  static void OnPointerAxis(void *data,
                            struct wl_pointer *wl_pointer,
                            uint32_t time,
                            uint32_t axis,
                            wl_fixed_t value);

  static void OnPointerFrame(void *data,
                             struct wl_pointer *wl_pointer);

  static void OnPointerAxisSource(void *data,
                                  struct wl_pointer *wl_pointer,
                                  uint32_t axis_source);

  static void OnPointerAxisStop(void *data,
                                struct wl_pointer *wl_pointer,
                                uint32_t time,
                                uint32_t axis);

  static void OnPointerAxisDiscrete(void *data,
                                    struct wl_pointer *wl_pointer,
                                    uint32_t axis,
                                    int32_t discrete);

  // keyboard:

  static void OnKeyboardKeymap(void *data,
                               struct wl_keyboard *wl_keyboard,
                               uint32_t format,
                               int32_t fd,
                               uint32_t size);

  static void OnKeyboardEnter(void *data,
                              struct wl_keyboard *wl_keyboard,
                              uint32_t serial,
                              struct wl_surface *wl_surface,
                              struct wl_array *keys);

  static void OnKeyboardLeave(void *data,
                              struct wl_keyboard *wl_keyboard,
                              uint32_t serial,
                              struct wl_surface *wl_surface);

  static void OnKeyboardKey(void *data,
                            struct wl_keyboard *wl_keyboard,
                            uint32_t serial,
                            uint32_t time,
                            uint32_t key,
                            uint32_t state);

  static void OnKeyboardModifiers(void *data,
                                  struct wl_keyboard *wl_keyboard,
                                  uint32_t serial,
                                  uint32_t mods_depressed,
                                  uint32_t mods_latched,
                                  uint32_t mods_locked,
                                  uint32_t group);

  static void OnKeyboardRepeatInfo(void *data,
                                   struct wl_keyboard *wl_keyboard,
                                   int32_t rate,
                                   int32_t delay);

  // touch:


  static void OnTouchDown(void *data,
                          struct wl_touch *wl_touch,
                          uint32_t serial,
                          uint32_t time,
                          struct wl_surface *surface,
                          int32_t id,
                          wl_fixed_t x,
                          wl_fixed_t y);

  static void OnTouchUp(void *data,
                        struct wl_touch *wl_touch,
                        uint32_t serial,
                        uint32_t time,
                        int32_t id);

  static void OnTouchMotion(void *data,
                            struct wl_touch *wl_touch,
                            uint32_t time,
                            int32_t id,
                            wl_fixed_t x,
                            wl_fixed_t y);

  static void OnTouchFrame(void *data,
                           struct wl_touch *wl_touch);

  static void OnTouchCancel(void *data,
                            struct wl_touch *wl_touch);

  static const struct wl_seat_listener kSeatListener;

  static const struct wl_pointer_listener kPointerListener;

  static const struct wl_keyboard_listener kKeyboardListener;

  static const struct wl_touch_listener kTouchListener;

};

} // namespace gui
} // namespace wiztk

#endif //WIZTK_INPUT_PRIVATE_HPP
