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

#ifndef WIZTK_ASYNC_MESSAGE_HPP_
#define WIZTK_ASYNC_MESSAGE_HPP_

#include "wiztk/base/binode.hpp"

namespace wiztk {
namespace async {

// Forward declaration:
class Message;

namespace internal {

/**
 * @ingroup async_intern
 * @brief Represents a bi-directional message node.
 */
class WIZTK_NO_EXPORT MessageTraits : public base::Binode<MessageTraits> {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(MessageTraits);

  explicit MessageTraits(Message *message)
      : message_(message) {}

  ~MessageTraits() final = default;

  Message *message() const { return message_; }

 private:

  Message *message_ = nullptr;

};

} // namespace internal

/**
 * @ingroup async
 * @brief A message which can be lined up and executed in a message queue.
 */
class WIZTK_EXPORT Message {

  friend class MessageQueue;
  friend class Scheduler;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Message);

  Message()
      : traits_(this) {}

  virtual ~Message() = default;

  virtual void Exec() {};

  bool IsQueued() const {
    return traits_.is_linked();
  }

  void Unlink() {
    traits_.unlink();
  }

 private:

  internal::MessageTraits traits_;

};

} // namespace async
} // namespace wiztk

#endif // WIZTK_ASYNC_ABSTRACT_EVENT_HPP_
