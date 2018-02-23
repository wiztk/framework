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

#ifndef WIZTK_ASYNC_INTERNAL_QUIT_EVENT_HPP_
#define WIZTK_ASYNC_INTERNAL_QUIT_EVENT_HPP_

#include "wiztk/async/event-loop.hpp"

namespace wiztk {
namespace async {

class EventLoop::QuitEvent : public AbstractEvent {

 public:

  QuitEvent() = delete;

  explicit QuitEvent(EventLoop *event_loop);

  ~QuitEvent() final;

  void Run(uint32_t events) final;

  int event_fd() const { return event_fd_; }

 private:

  EventLoop *event_loop_ = nullptr;

  int event_fd_ = -1;

};

}
}

#endif // WIZTK_ASYNC_INTERNAL_QUIT_EVENT_HPP_
