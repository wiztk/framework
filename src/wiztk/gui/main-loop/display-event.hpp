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

#ifndef WIZTK_GUI_MAIN_LOOP_DISPLAY_EVENT_HPP_
#define WIZTK_GUI_MAIN_LOOP_DISPLAY_EVENT_HPP_

#include "wiztk/gui/main-loop.hpp"

namespace wiztk {
namespace gui {

class MainLoop::DisplayEvent : public async::AbstractEvent {

 public:

  explicit DisplayEvent(MainLoop *main_loop);

  ~DisplayEvent() final;

 protected:

  void Run(uint32_t events) final;

 private:

  MainLoop *main_loop_ = nullptr;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_MAIN_LOOP_DISPLAY_EVENT_HPP_
