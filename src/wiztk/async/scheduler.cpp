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

#include "wiztk/async/scheduler.hpp"

#include "wiztk/async/event-loop.hpp"

namespace wiztk {
namespace async {

Scheduler::Scheduler() {
  event_loop_ = EventLoop::GetCurrent();
  if (nullptr == event_loop_)
    throw std::runtime_error("Error! Cannot get the event loop in this thread!");
}

Scheduler::Scheduler(EventLoop *event_loop)
    : event_loop_(event_loop) {
  if (nullptr == event_loop_)
    throw std::runtime_error("Error! Cannot get the event loop in this thread!");
}

Scheduler::~Scheduler() = default;

void Scheduler::PostMessage(Message *message) {
  event_loop_->message_queue_.PushBack(message);
}

void Scheduler::PostMessageAfter(Message *a, Message *b) {
  if (!a->IsQueued()) return;

  a->traits_.push_back(&b->traits_);
}

} // namespace async
} // namespace wiztk
