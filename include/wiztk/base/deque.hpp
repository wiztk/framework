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
template<typename T>
class Deque;

namespace internal {

/**
 * @ingroup base_intern
 * @brief Endpoint used in Deque.
 */
class DequeEndpoint : public BinodeBase {

  template<typename T> friend
  class wiztk::base::Deque;

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
class DequeNodeBase : public BinodeBase {};

/**
 * @ingroup base
 * @brief Bidirectional node used in Deque only.
 * @tparam T
 */
template<typename T>
class DequeNode : public DequeNodeBase {

  template<typename R> friend
  class Deque;

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
 * @brief A simple double-ended queue container
 * @tparam T Must be a subclass of DequeNode
 */
template<typename T>
class Deque {

  template<typename R> friend
  class CountedDeque;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Deque);

  typedef std::function<void(BinodeBase * )> DeleterType;

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

    T *operator->() const noexcept { return get(); }

    T *get() const noexcept {
      return nullptr == current_->previous_ ?
             nullptr : (nullptr == current_->next_ ?
                        nullptr : (static_cast<T *>(current_)));
    }

    explicit operator bool() const {
      return nullptr == current_ ?
             false : (nullptr == current_->previous_ ?
                      false : (nullptr != current_->next_));
    }

   private:

    BinodeBase *current_;

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

    ~ConstIterator() = default;

    ConstIterator &operator=(const ConstIterator &) = default;

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

    const T *operator->() const noexcept { return get(); }

    const T *get() const noexcept {
      return nullptr == current_->previous_ ?
             nullptr : (nullptr == current_->next_ ?
                        nullptr : (static_cast<const T *>(current_)));
    }

    explicit operator bool() const {
      return nullptr == current_ ?
             false : (nullptr == current_->previous_ ?
                      false : (nullptr != current_->next_));
    }

   private:

    const BinodeBase *current_;

  };

  Deque();

  explicit Deque(const DeleterType &deleter);

  virtual ~Deque();

  void push_front(DequeNodeBase *item) { BinodeBase::PushBack(&head_, item); }

  void push_back(DequeNodeBase *item) { BinodeBase::PushFront(&tail_, item); }

  void insert(DequeNodeBase *item, int index = 0);

  size_t count() const;

  bool is_empty() const;

  /**
   * @brief Use the member deleter to remove all nodes.
   */
  void clear();

  void clear(const DeleterType &deleter);

  T *operator[](int index) const {
    return at(index);
  }

  T *at(int index) const;

  Iterator begin() const { return Iterator(head_.next_); }

  ConstIterator cbegin() const { return ConstIterator(head_.next_); }

  Iterator rbegin() const { return Iterator(tail_.previous_); }

  ConstIterator crbegin() const { return ConstIterator(tail_.previous_); }

  Iterator end() { return Iterator(&tail_); }

  ConstIterator cend() const { return ConstIterator(&tail_); }

  Iterator rend() { return Iterator(&head_); }

  ConstIterator crend() const { return ConstIterator(&head_); }

 protected:

  internal::DequeEndpoint head_;
  internal::DequeEndpoint tail_;

  std::function<void(BinodeBase * )> deleter_;
};

template<typename T>
Deque<T>::Deque() {
  head_.next_ = &tail_;
  tail_.previous_ = &head_;
}

template<typename T>
Deque<T>::Deque(const DeleterType &deleter)
    : Deque() {
  deleter_ = deleter;
}

template<typename T>
Deque<T>::~Deque() {
  clear();
}

template<typename T>
void Deque<T>::insert(DequeNodeBase *item, int index) {
  if (index >= 0) {
    BinodeBase *p = head_.next_;
    while ((&tail_ != p) && (index > 0)) {
      p = p->next_;
      index--;
    }
    BinodeBase::PushFront(p, item);
  } else {
    BinodeBase *p = tail_.previous_;
    while ((&head_ != p) && (index < -1)) {
      p = p->previous_;
      index++;
    }
    BinodeBase::PushBack(p, item);
  }
}

template<typename T>
size_t Deque<T>::count() const {
  size_t size = 0;

  BinodeBase *element = head_.next_;
  while (element != &tail_) {
    ++size;
    element = element->next_;
  }

  return size;
}

template<typename T>
bool Deque<T>::is_empty() const {
  return head_.next_ == &tail_;
}

template<typename T>
void Deque<T>::clear() {
  BinodeBase *tmp = nullptr;
  while (!is_empty()) {
    tmp = begin().get();
    BinodeBase::Unlink(tmp);
    if (deleter_) deleter_(tmp);
  }
}

template<typename T>
void Deque<T>::clear(const DeleterType &deleter) {
  BinodeBase *tmp = nullptr;
  while (!is_empty()) {
    tmp = begin().get();
    BinodeBase::Unlink(tmp);
    if (deleter) deleter(tmp);
  }
}

template<typename T>
T *Deque<T>::at(int index) const {
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
