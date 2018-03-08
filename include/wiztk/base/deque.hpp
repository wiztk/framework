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

#ifndef WIZTK_BASE_DEQUE_HPP_
#define WIZTK_BASE_DEQUE_HPP_

#include "wiztk/base/binode.hpp"

#include <cstddef>
#include <functional>

namespace wiztk {
namespace base {

// Forward declarations:
template<typename T, typename D>
class Deque;

namespace internal {

/**
 * @ingroup base_intern
 * @brief Endpoint used in Deque.
 */
class WIZTK_NO_EXPORT DequeEndpoint : public BinodeBase {

  template<typename T, typename D> friend
  class base::Deque;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(DequeEndpoint);

 protected:

  DequeEndpoint() = default;

 private:

  ~DequeEndpoint() final = default;

};

} // namespace internal

/**
 * @ingroup base
 * @brief Bidirectional node used in Deque.
 */
class WIZTK_EXPORT DequeNodeBase : public BinodeBase {};

/**
 * @ingroup base
 * @brief A helper template class represents a DequeNodeBase with default
 * methods.
 * @tparam T Must be a subclass of DequeNodeBase.
 *
 * You usually use this template class in this way:
 *
 * @code
 * class MyNode : public DequeNode<MyNode>;
 * @endcode
 *
 * This is the same as:
 *
 * @code
 * class MyNode : public DequeNodeBase {
 *
 *   public:
 *
 *     // ...
 *
 *     void push_back(MyNode *other);
 *     void push_front(MyNode *other);
 *     void unlink();
 *     bool is_linked() const;
 *     MyNode* previous() const;
 *     MyNode* next() const;
 *
 * };
 * @endcode
 */
template<typename T>
class WIZTK_EXPORT DequeNode : public DequeNodeBase {

 public:

  DequeNode() = default;

  ~DequeNode() override = default;

  void push_back(T *other) { PushBack(this, other); }

  void push_front(T *other) { PushFront(this, other); }

  void unlink() { Unlink(this); }

  bool is_linked() const { return IsLinked(this); }

  T *previous() const { return dynamic_cast<T *>(previous_); }

  T *next() const { return dynamic_cast<T *>(next_); }

};

/**
 * @ingroup base
 * @brief Default class used to delete elements in deque when cleaning up.
 */
class DefaultDequeDeleter {

 public:

  DefaultDequeDeleter() = default;
  ~DefaultDequeDeleter() = default;

  void operator()(BinodeBase *obj) {
    delete obj;
  }

};

/**
 * @ingroup base
 * @brief A simple double-ended queue container.
 * @tparam T Must be a subclass of DequeNodeBase
 *
 * This class represents a double-ended queue which contains pointers to
 * DequeNodeBase.
 *
 * Internally it contains 2 node objects as the endpoints.
 *
 * When a Deque object is destroyed, it will remove all nodes but not delete
 * them by default. You can assign a deleter with in constructor, or use
 * particular clear(const DeleteType&) method.
 */
template<typename T, typename Deleter = DefaultDequeDeleter>
class Deque {

 public:

  /** @brief This class is non-copyable and non-movable */
  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Deque);

  /**
   * @brief A nested iterator for deque
   */
  class Iterator {

   public:

    explicit Iterator(BinodeBase *obj = nullptr)
        : current_(obj) {}

    Iterator(const Iterator &) = default;
    Iterator(Iterator &&) noexcept = default;
    ~Iterator() = default;
    Iterator &operator=(const Iterator &) = default;
    Iterator &operator=(Iterator &&) noexcept = default;

    Iterator &operator++() {
      current_ = current_->next_;
      return *this;
    }

    Iterator operator++(int) { return Iterator(current_->next_); }

    Iterator &operator--() {
      current_ = current_->previous_;
      return *this;
    }

    Iterator operator--(int) { return Iterator(current_->previous_); }

    void push_front(T *element) { BinodeBase::PushFront(current_, element); }

    void push_back(T *element) { BinodeBase::PushBack(current_, element); }

    /**
     * @brief Unlink the bi-node object and invalidate this iterator
     *
     * @note Once this method is called, this iterator is invalidated and
     * should not be used, but it can be assigned to another one.
     */
    void remove() {
      BinodeBase::Unlink(current_);
      current_ = nullptr;
    }

    bool operator==(const Iterator &other) const { return current_ == other.current_; }

    bool operator==(const T *element) const { return current_ == element; }

    bool operator!=(const Iterator &other) const { return current_ != other.current_; }

    bool operator!=(const T *element) const { return current_ != element; }

    T *operator->() const { return get(); }

    T *get() const { return static_cast<T *>(current_); }

    explicit operator bool() const {
      return nullptr == current_ ?
             false : (nullptr == current_->previous_ ?
                      false : (nullptr != current_->next_));
    }

   private:

    BinodeBase *current_ = nullptr;

  };

  /**
   * @brief A nested const iterator class
   */
  class ConstIterator {

   public:

    ConstIterator() = delete;

    explicit ConstIterator(const BinodeBase *element = nullptr)
        : current_(element) {}

    ConstIterator(const ConstIterator &) = default;
    ConstIterator(ConstIterator &&) noexcept = default;
    ~ConstIterator() = default;
    ConstIterator &operator=(const ConstIterator &) = default;
    ConstIterator &operator=(ConstIterator &&) = default;

    ConstIterator &operator++() {
      current_ = current_->next_;
      return *this;
    }

    ConstIterator operator++(int) { return ConstIterator(current_->next_); }

    ConstIterator &operator--() {
      current_ = current_->previous_;
      return *this;
    }

    ConstIterator operator--(int) { return ConstIterator(current_->previous_); }

    bool operator==(const ConstIterator &other) const { return current_ == other.current_; }

    bool operator==(const T *element) const { return current_ == element; }

    bool operator!=(const ConstIterator &other) const { return current_ != other.current_; }

    bool operator!=(const T *element) const { return current_ != element; }

    const T *operator->() const { return get(); }

    const T *get() const { return static_cast<const T *>(current_); }

    explicit operator bool() const {
      return nullptr == current_ ?
             false : (nullptr == current_->previous_ ?
                      false : (nullptr != current_->next_));
    }

   private:

    const BinodeBase *current_ = nullptr;

  };

  /**
   * @brief Reverse iterator.
   */
  class ReverseIterator {

   public:

    explicit ReverseIterator(BinodeBase *obj = nullptr)
        : current_(obj) {}

    ReverseIterator(const ReverseIterator &) = default;
    ReverseIterator(ReverseIterator &&) noexcept = default;
    ~ReverseIterator() = default;
    ReverseIterator &operator=(const ReverseIterator &) = default;
    ReverseIterator &operator=(ReverseIterator &&) noexcept = default;

    ReverseIterator &operator++() {
      current_ = current_->previous_;
      return *this;
    }

    ReverseIterator operator++(int) { return ReverseIterator(current_->previous_); }

    ReverseIterator &operator--() {
      current_ = current_->next_;
      return *this;
    }

    ReverseIterator operator--(int) { return ReverseIterator(current_->next_); }

    void push_front(T *element) { BinodeBase::PushFront(current_, element); }

    void push_back(T *element) { BinodeBase::PushBack(current_, element); }

    /**
     * @brief Unlink the bi-node object and invalidate this iterator
     *
     * @note Once this method is called, this iterator is invalidated and
     * should not be used, but it can be assigned to another one.
     */
    void remove() {
      BinodeBase::Unlink(current_);
      current_ = nullptr;
    }

    bool operator==(const ReverseIterator &other) const { return current_ == other.current_; }

    bool operator==(const T *element) const { return current_ == element; }

    bool operator!=(const ReverseIterator &other) const { return current_ != other.current_; }

    bool operator!=(const T *element) const { return current_ != element; }

    T *operator->() const { return get(); }

    T *get() const { return static_cast<T *>(current_); }

    explicit operator bool() const {
      return nullptr == current_ ?
             false : (nullptr == current_->previous_ ?
                      false : (nullptr != current_->next_));
    }

   private:

    BinodeBase *current_ = nullptr;

  };

  /**
   * @brief A nested const iterator class
   */
  class ConstReverseIterator {

   public:

    ConstReverseIterator() = delete;

    explicit ConstReverseIterator(const BinodeBase *element = nullptr)
        : current_(element) {}

    ConstReverseIterator(const ConstReverseIterator &) = default;
    ConstReverseIterator(ConstReverseIterator &&) noexcept = default;
    ~ConstReverseIterator() = default;
    ConstReverseIterator &operator=(const ConstReverseIterator &) = default;
    ConstReverseIterator &operator=(ConstReverseIterator &&) noexcept  = default;

    ConstReverseIterator &operator++() {
      current_ = current_->previous_;
      return *this;
    }

    ConstReverseIterator operator++(int) { return ConstReverseIterator(current_->previous_); }

    ConstReverseIterator &operator--() {
      current_ = current_->next_;
      return *this;
    }

    ConstReverseIterator operator--(int) { return ConstReverseIterator(current_->next_); }

    bool operator==(const ConstReverseIterator &other) const { return current_ == other.current_; }

    bool operator==(const T *element) const { return current_ == element; }

    bool operator!=(const ConstReverseIterator &other) const { return current_ != other.current_; }

    bool operator!=(const T *element) const { return current_ != element; }

    const T *operator->() const { return get(); }

    const T *get() const { return static_cast<const T *>(current_); }

    explicit operator bool() const {
      return nullptr == current_ ?
             false : (nullptr == current_->previous_ ?
                      false : (nullptr != current_->next_));
    }

   private:

    const BinodeBase *current_ = nullptr;

  };

 public:

  /**
   * @brief Default constructor.
   */
  Deque();

  /**
   * @brief Constructor by given deleter object.
   * @param deleter
   */
  explicit Deque(const Deleter &deleter);

  /**
   * @brief Destructor.
   */
  virtual ~Deque();

  /**
   * @brief Push a node at the beginning.
   * @param node
   */
  void push_front(T *node) { BinodeBase::PushBack(&head_, node); }

  /**
   * @brief Push a node at the end.
   * @param node
   */
  void push_back(T *node) { BinodeBase::PushFront(&tail_, node); }

  /**
   * @brief Insert a node at the given index.
   * @param node
   * @param index
   */
  void insert(T *node, int index = 0);

  /**
   * @brief Get the count of all nodes contained in this Deque.
   * @return The count number.
   */
  size_t count() const;

  /**
   * @brief Returns if this deque is empty (count() == 0).
   * @return
   *   - true: Is empty.
   *   - false: Not empty.
   */
  bool is_empty() const;

  /**
   * @brief Use the member deleter to remove all nodes.
   */
  void clear();

  /**
   * @brief Use the particular deleter to remove all nodes.
   */
  void clear(const Deleter &deleter);

  /**
   * @brief Get the node at the index.
   * @param index
   * @return
   */
  T *operator[](int index) const {
    return at(index);
  }

  /**
   * @brief Get the node at the index.
   * @param index
   * @return
   */
  T *at(int index) const;

  /**
   * @brief Returns an iterator points to the first node.
   * @return
   */
  Iterator begin() const { return Iterator(head_.next_); }

  /**
   * @brief Returns a const iterator points to the first node.
   * @return
   */
  ConstIterator cbegin() const { return ConstIterator(head_.next_); }

  /**
   * @brief Returns an iterator points to the end.
   * @return
   */
  Iterator end() const {
    const BinodeBase *p = &tail_;
    return Iterator(const_cast<BinodeBase *>(p));
  }

  /**
   * @brief Returns a const iterator points to the end.
   * @return
   */
  ConstIterator cend() const { return ConstIterator(&tail_); }

  /**
   * @brief Returns an iterator points to the reverse first node.
   * @return
   */
  ReverseIterator rbegin() const { return ReverseIterator(tail_.previous_); }

  /**
   * @brief Returns a const iterator points to the reverse first node.
   * @return
   */
  ConstReverseIterator crbegin() const { return ConstReverseIterator(tail_.previous_); }

  /**
   * @brief Returns an iterator points to the reverse end.
   * @return
   */
  ReverseIterator rend() const {
    const BinodeBase *p = &head_;
    return ReverseIterator(const_cast<BinodeBase *>(p));
  }

  /**
   * @brief Returns a const iterator points to the reverse end.
   * @return
   */
  ConstReverseIterator crend() const { return ConstReverseIterator(&head_); }

 protected:

  /**
   * @brief The head of this deque.
   */
  internal::DequeEndpoint head_;

  /**
   * @brief The tail of this deque.
   */
  internal::DequeEndpoint tail_;

  /**
   * @brief A function object to process node when removing it in clear();
   */
  Deleter deleter_;

};

template<typename T, typename Deleter>
Deque<T, Deleter>::Deque() {
  head_.next_ = &tail_;
  tail_.previous_ = &head_;
}

template<typename T, typename Deleter>
Deque<T, Deleter>::Deque(const Deleter &deleter)
    : Deque() {
  deleter_ = deleter;
}

template<typename T, typename Deleter>
Deque<T, Deleter>::~Deque() {
  clear();
}

template<typename T, typename Deleter>
void Deque<T, Deleter>::insert(T *node, int index) {
  if (index >= 0) {
    BinodeBase *p = head_.next_;
    while ((&tail_ != p) && (index > 0)) {
      p = p->next_;
      index--;
    }
    BinodeBase::PushFront(p, node);
  } else {
    BinodeBase *p = tail_.previous_;
    while ((&head_ != p) && (index < -1)) {
      p = p->previous_;
      index++;
    }
    BinodeBase::PushBack(p, node);
  }
}

template<typename T, typename Deleter>
size_t Deque<T, Deleter>::count() const {
  size_t size = 0;

  BinodeBase *element = head_.next_;
  while (element != &tail_) {
    ++size;
    element = element->next_;
  }

  return size;
}

template<typename T, typename Deleter>
bool Deque<T, Deleter>::is_empty() const {
  bool ret = head_.next_ == &tail_;

  if (ret) {
    _ASSERT(tail_.previous_ == &head_);
  }

  return ret;
}

template<typename T, typename Deleter>
void Deque<T, Deleter>::clear() {
  BinodeBase *tmp = nullptr;
  while (!is_empty()) {
    tmp = begin().get();
    BinodeBase::Unlink(tmp);
    deleter_(tmp);
  }
}

template<typename T, typename Deleter>
void Deque<T, Deleter>::clear(const Deleter &deleter) {
  BinodeBase *tmp = nullptr;
  while (!is_empty()) {
    tmp = begin().get();
    BinodeBase::Unlink(tmp);
    deleter(tmp);
  }
}

template<typename T, typename Deleter>
T *Deque<T, Deleter>::at(int index) const {
  BinodeBase *p = nullptr;

  if (index >= 0) {
    p = head_.next_;
    while ((&tail_ != p) && (index > 0)) {
      p = p->next_;
      index--;
    }
  } else {
    p = tail_.previous_;
    while ((&head_ != p) && (index < -1)) {
      p = p->previous_;
      index++;
    }
  }

  return static_cast<T *>(p);
}

} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_DEQUE_HPP_
