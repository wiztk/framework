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

#include "internal/abstract-view_private.hpp"
#include "internal/abstract-event-handler_private.hpp"

#include "wiztk/base/memory.hpp"

namespace wiztk {
namespace gui {

AbstractEventHandler::AbstractEventHandler() {
  p_ = std::make_unique<Private>(this);
}

AbstractEventHandler::~AbstractEventHandler() {

}

void AbstractEventHandler::AuditDestroyingToken(base::detail::Token */*token*/) {

}

// --------------------

AbstractEventHandler::EventTask *AbstractEventHandler::EventTask::GetMouseTask(const AbstractEventHandler *event_hander) {
  return &event_hander->p_->mouse_task;
}

AbstractEventHandler::EventTask *AbstractEventHandler::EventTask::GetMouseMotionTask(const AbstractEventHandler *event_handler) {
  return &event_handler->p_->mouse_motion_task;
}

} // namespace gui
} // namespace wiztk

