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

#ifndef WIZTK_ASYNC_MESSAGE_QUEUE_HPP_
#define WIZTK_ASYNC_MESSAGE_QUEUE_HPP_

#include "wiztk/base/deque.hpp"
#include "wiztk/async/message.hpp"

namespace wiztk {
namespace async {

// Forward declaration:
class MessageQueue;

namespace internal {

/**
 * @ingroup async_intern
 */
class WIZTK_NO_EXPORT MessageQueueTraits : public base::Deque<MessageTraits> {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(MessageQueueTraits);

  explicit MessageQueueTraits(MessageQueue *event_queue)
      : message_queue_(event_queue) {}

  ~MessageQueueTraits() final = default;

 private:

  MessageQueue *message_queue_ = nullptr;

};

} // namespace internal

/**
 * @ingroup async
 * @brief A first in - first out event queue.
 */
class WIZTK_EXPORT MessageQueue {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(MessageQueue);

  MessageQueue();

  ~MessageQueue() = default;

  void PushFront(Message *message);

  void PushBack(Message *message);

  Message *PopFront();

  Message *PopBack();

 private:

  internal::MessageQueueTraits traits_;

};

} // namespace async
} // namespace wiztk

#endif // WIZTK_ASYNC_MESSAGE_QUEUE_HPP_
