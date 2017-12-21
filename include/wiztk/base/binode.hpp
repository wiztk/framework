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

/**
 * @file binode.hpp
 * @brief Header file for bidirectional node classes.
 */

#ifndef WIZTK_BASE_BINODE_HPP_
#define WIZTK_BASE_BINODE_HPP_

#include "wiztk/base/macros.hpp"

namespace wiztk {
namespace base {

/**
 * @ingroup base
 * @brief Base class for bidirectional node.
 *
 * You cannot initialize an instance of this base class directly. Instead, create and
 * use a subclass.
 */
class WIZTK_EXPORT BinodeBase {

  template<typename T> friend
  class Deque;

 public:

  WIZTK_DECLARE_NONCOPYABLE(BinodeBase);

  /**
   * @brief Move constructor.
   * @param other
   */
  BinodeBase(BinodeBase &&other) noexcept
      : previous_(other.previous_), next_(other.next_) {
    other.previous_ = nullptr;
    other.next_ = nullptr;
  }

  /**
   * @brief Destructor.
   *
   * The destructor will break the link to other node.
   */
  virtual ~BinodeBase();

  /**
   * @brief Move operator.
   * @param other
   * @return
   */
  BinodeBase &operator=(BinodeBase &&other) noexcept {
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
  void PushFront(BinodeBase *other);

  /**
   * @brief Push another node to the back
   * @param other
   */
  void PushBack(BinodeBase *other);

  /**
   * @brief Break the link to both the previous and next node
   */
  void Unlink();

 protected:

  /**
    * @brief Default constructor
    */
  BinodeBase() = default;

  BinodeBase *previous_ = nullptr;
  BinodeBase *next_ = nullptr;

};

/**
 * @ingroup base
 * @brief A template class represents a bidirectional node.
 * @tparam T Usually a type of subclass
 */
template<typename T>
class WIZTK_EXPORT Binode : protected BinodeBase {

  template<typename R> friend
  class Deque;

 public:

  WIZTK_DECLARE_NONCOPYABLE(Binode);

  Binode() = default;

  Binode(Binode &&) noexcept = default;

  ~Binode() override = default;

  Binode &operator=(Binode &&) noexcept = default;

  inline void push_back(T *node) { PushBack(node); }

  inline void push_front(T *node) { PushFront(node); }

  inline void unlink() { Unlink(); }

  inline bool is_linked() const { return IsLinked(); }

  inline T *previous() const { return static_cast<T *>(previous_); }

  inline T *next() const { return static_cast<T *>(next_); }

};

} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_BINODE_HPP_
