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

#include "wiztk/base/property.hpp"

#include "main-loop_display-event.hpp"
#include "main-loop_private.hpp"
#include "display_proxy.hpp"

namespace wiztk {
namespace gui {

MainLoop::DisplayEvent::DisplayEvent(MainLoop *main_loop)
    : main_loop_(main_loop) {

}

MainLoop::DisplayEvent::~DisplayEvent() = default;

void MainLoop::DisplayEvent::Run(uint32_t events) {
  if (events & EPOLLERR || events & EPOLLHUP) {
    main_loop_->Quit();
    return;
  }
  if (events & EPOLLIN) {
    if (wl_display_dispatch(main_loop_->p_->wl_display_) == -1) {
      main_loop_->Quit();
      return;
    }
  }
  if (events & EPOLLOUT) {
    int ret = wl_display_flush(main_loop_->p_->wl_display_);
    if (ret == 0) {
      int display_fd = wl_display_get_fd(main_loop_->p_->wl_display_);
      main_loop_->ModifyWatchedFileDescriptor(display_fd,
                                              this,
                                              EPOLLIN | EPOLLERR | EPOLLHUP);
    } else if (ret == -1 && errno != EAGAIN) {
      main_loop_->Quit();
      return;
    }
  }
}

}
}
