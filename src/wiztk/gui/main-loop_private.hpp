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

#ifndef WIZTK_GUI_MAIN_LOOP_PRIVATE_HPP_
#define WIZTK_GUI_MAIN_LOOP_PRIVATE_HPP_

#include "main-loop_signal-event.hpp"
#include "main-loop_display-event.hpp"

#include "wiztk/gui/main-loop.hpp"

#include <wayland-client.h>

namespace wiztk {
namespace gui {

struct MainLoop::_Private {

  explicit _Private(MainLoop *main_loop);

  ~_Private() = default;

  struct wl_display *wl_display = nullptr;

  SignalEvent signal_event;

  DisplayEvent wayland_event;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_MAIN_LOOP_PRIVATE_HPP_
