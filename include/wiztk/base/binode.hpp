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

#ifndef WIZTK_BASE_BINODE_HPP_
#define WIZTK_BASE_BINODE_HPP_

#include "wiztk/base/macros.hpp"

namespace wiztk {
namespace base {

/**
 * @ingroup base
 * @brief A bidirectional node used in deque or custom list
 *
 * You usually don't use this class directly. Instead, create and use a subclass.
 *
 * A BiNode object can be linked to another by using the PushBack(), PushFront().
 * When needed, use a Deque to manage all nodes, for example:
 * @code
 *  class CustomNode: public base::BiNode {}
 *
 *  base::Deque<CustomNode> deque;
 *  auto node1 = new CustomNode;
 *  auto node2 = new CustomNode;
 *  deque.PushBack(node1);
 *  deque.Insert(node2);
 * @endcode
 */
class Binode {

  template<typename T> friend
  class Deque;

 public:

  WIZTK_DECLARE_NONCOPYABLE(Binode);

  /**
   * @brief Default constructor
   */
  Binode() = default;

  Binode(Binode &&other) noexcept
      : previous_(other.previous_), next_(other.next_) {
    other.previous_ = nullptr;
    other.next_ = nullptr;
  }

  /**
   * @brief Destructor.
   *
   * The destructor will break the link to other node.
   */
  virtual ~Binode();

  Binode &operator=(Binode &&other) noexcept {
    previous_ = other.previous_;
    next_ = other.next_;
    other.previous_ = nullptr;
    other.next_ = nullptr;
    return *this;
  }

  /**
   * @brief Check if this node is linked to another
   * @return
   */
  bool IsLinked() const {
    return (nullptr != previous_) || (nullptr != next_);
  }

  /**
   * @brief Push another node to the front
   * @param other
   */
  void PushFront(Binode *other);

  /**
   * @brief Push another node to the back
   * @param other
   */
  void PushBack(Binode *other);

  /**
   * @brief Break the link to both the previous and next node
   */
  void Unlink();

 protected:

  virtual void OnUnlinked() { /* Override this in subclass */ }

  virtual void OnLinked() { /* Override this in subclass */ }

  Binode *previous_ = nullptr;
  Binode *next_ = nullptr;

};

} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_BINODE_HPP_
