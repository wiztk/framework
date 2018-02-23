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

#include "wiztk/async/message-queue.hpp"

namespace wiztk {
namespace async {

MessageQueue::MessageQueue()
    : traits_(this) {
}

void MessageQueue::PushFront(Message *message) {
  traits_.push_front(&message->traits_);
}

void MessageQueue::PushBack(Message *message) {
  traits_.push_back(&message->traits_);
}

Message *MessageQueue::PopFront() {
  if (traits_.is_empty()) return nullptr;

  auto it = traits_.begin();
  it->unlink();
  return it->message();
}

Message *MessageQueue::PopBack() {
  if (traits_.is_empty()) return nullptr;

  auto it = traits_.rbegin();
  it->unlink();
  return it->message();
}

}
}