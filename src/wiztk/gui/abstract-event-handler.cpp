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

#include "abstract-event-handler/private.hpp"

namespace wiztk {
namespace gui {

using base::String;

AbstractEventHandler::MouseTask *
AbstractEventHandler::MouseTask::Get(const AbstractEventHandler *event_hander) {
  return &event_hander->__PROPERTY__(mouse_event_node);
}

AbstractEventHandler::MouseMotionTask *
AbstractEventHandler::MouseMotionTask::Get(const AbstractEventHandler *event_handler) {
  return &event_handler->__PROPERTY__(mouse_motion_event_node);
}

// --------------------

AbstractEventHandler::AbstractEventHandler() {
  p_ = std::make_unique<Private>(this);
}

AbstractEventHandler::~AbstractEventHandler() = default;

const String &AbstractEventHandler::GetName() const {
  return __PROPERTY__(name);
}

void AbstractEventHandler::SetName(const String &name) {
  __PROPERTY__(name) = name;
}

} // namespace gui
} // namespace wiztk

