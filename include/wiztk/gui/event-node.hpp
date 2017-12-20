/*
 * Copyright 2017 The WizTK Authors.
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

#ifndef WIZTK_GUI_EVENT_NODE_HPP_
#define WIZTK_GUI_EVENT_NODE_HPP_

#include "wiztk/base/macros.hpp"
#include "wiztk/base/binode.hpp"

namespace wiztk {
namespace gui {

/**
 * @ingroup gui
 * @brief A template class represents an event in queue.
 * @tparam T Usually a type of subclass
 */
template<typename T>
class EventNode : protected base::Binode {

  template<typename R> friend
  class base::Deque;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(EventNode);

  EventNode() = default;

  ~EventNode() override = default;

  inline void push_back(T *node) { PushBack(node); }

  inline void push_front(T *node) { PushFront(node); }

  inline void unlink() { Unlink(); }

  inline bool is_linked() const { return IsLinked(); }

  inline T *previous() const { return static_cast<T *>(previous_); }

  inline T *next() const { return static_cast<T *>(next_); }

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_EVENT_NODE_HPP_
