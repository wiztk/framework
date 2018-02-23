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
 * @file counted-deque.hpp
 *
 * @brief Header file for fast countable deque.
 */

#ifndef WIZTK_BASE_COUNTED_DEQUE_HPP_
#define WIZTK_BASE_COUNTED_DEQUE_HPP_

#include "wiztk/base/deque.hpp"

namespace wiztk {
namespace base {

// Foward declaration:
class CountedDequeNodeBase;
template<typename T>
class CountedDequeNode;
class CountedDequeBase;
template<typename T>
class CountedDeque;

namespace internal {

/**
 * @ingroup base_intern
 * @brief Endpoint used in CountedDeque.
 */
class WIZTK_NO_EXPORT CountedDequeEndpoint : public BinodeBase {

  friend class base::CountedDequeBase;

  template<typename T> friend
  class base::CountedDeque;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(CountedDequeEndpoint);

 protected:

  CountedDequeEndpoint() = default;

 private:

  ~CountedDequeEndpoint() final = default;

};

/**
 * @ingroup base_intern
 * @brief Bidrectional node represents traits in CountedDequeNode.
 */
class WIZTK_NO_EXPORT CountedDequeNodeTraits : public BinodeBase {

  friend class base::CountedDequeNodeBase;
  template<typename T> friend
  class base::CountedDequeNode;
  template<typename T> friend
  class base::CountedDeque;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(CountedDequeNodeTraits);
  CountedDequeNodeTraits() = delete;

 public:

  explicit CountedDequeNodeTraits(CountedDequeNodeBase *node)
      : node_(node) {}

  ~CountedDequeNodeTraits() final;

 private:

  void ResetDeque();

  CountedDequeNodeBase *node_ = nullptr;

};

} // namespace internal

/**
 * @ingroup base
 * @brief The basic class for bidirectional node with pointer to a countable deque.
 */
class WIZTK_EXPORT CountedDequeNodeBase {

  friend class internal::CountedDequeNodeTraits;
  friend class CountedDequeBase;

  template<typename T> friend
  class CountedDeque;

 public:

  /**
   * @brief Destructor.
   *
   * This will decrease the count in deque.
   */
  virtual ~CountedDequeNodeBase() = default;

 protected:

  /**
   * @brief Default constructor.
   */
  CountedDequeNodeBase()
      : traits_(this) {}

  static void PushFront(CountedDequeNodeBase *node, CountedDequeNodeBase *other);

  static void PushBack(CountedDequeNodeBase *node, CountedDequeNodeBase *other);

  static void Unlink(CountedDequeNodeBase *node);

  /**
   * @brief Returns if a node is linked.
   * @param node
   * @return
   */
  static bool IsLinked(const CountedDequeNodeBase *node);

  /**
   * @brief A pointer to a fast countable deque object.
   */
  CountedDequeBase *deque_ = nullptr;

  internal::CountedDequeNodeTraits traits_;

};

/**
 * @ingroup base
 * @brief Template class for bidirectional node used in fast countable deque.
 * @tparam T Must be a subclass of CountedBinodeBase.
 */
template<typename T>
class WIZTK_EXPORT CountedDequeNode : public CountedDequeNodeBase {

 public:

  /**
   * @brief Default constructor.
   */
  CountedDequeNode() = default;

  /**
   * @brief Default destructor.
   */
  ~CountedDequeNode() override = default;

  void unlink();

  bool is_linked() const;

  T *previous() const {
    auto *previous = dynamic_cast<internal::CountedDequeNodeTraits *>(traits_.previous_);
    return nullptr == previous ? nullptr : static_cast<T *> (previous->node_);
  }

  T *next() const {
    auto *next = dynamic_cast<internal::CountedDequeNodeTraits *>(traits_.next_);
    return nullptr == next ? nullptr : static_cast<T *>(next->node_);
  }

  CountedDequeBase *deque() const { return deque_; }

};

/**
 * @ingroup base
 * @brief Counted deque base
 */
class WIZTK_EXPORT CountedDequeBase {

  friend class internal::CountedDequeNodeTraits;
  friend class CountedDequeNodeBase;

  template<typename T> friend
  class CountedDequeNode;

  template<typename T> friend
  class CountedDeque;

 public:

  typedef std::function<void(CountedDequeNodeBase *)> DeleterType;

  virtual ~CountedDequeBase();

 protected:

  internal::CountedDequeEndpoint head_;
  internal::CountedDequeEndpoint tail_;

  size_t count_ = 0;

  DeleterType deleter_;

 private:

  CountedDequeBase() {
    head_.next_ = &tail_;
    tail_.previous_ = &head_;
  }

  explicit CountedDequeBase(const DeleterType &deleter)
      : CountedDequeBase() {
    deleter_ = deleter;
  }

};

/**
 * @ingroup base
 * @brief Counted deque.
 * @tparam T Must be a subclass of CountedDequeNodeBase.
 */
template<typename T>
class WIZTK_EXPORT CountedDeque : public CountedDequeBase {

 public:

  /**
    * @brief A nested iterator for CountedDeque.
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

    bool operator==(const Iterator &other) const { return current_ == other.current_; }

    bool operator==(const T *element) const { return current_ == element; }

    bool operator!=(const Iterator &other) const { return current_ != other.current_; }

    bool operator!=(const T *element) const { return current_ != element; }

    T *operator->() const noexcept { return get(); }

    T *get() const noexcept {
      using Traits = internal::CountedDequeNodeTraits;
      return nullptr == current_->previous_ ?
             nullptr : (nullptr == current_->next_ ?
                        nullptr : static_cast<T *>(static_cast<Traits *>(current_)->node_));
    }

    explicit operator bool() const {
      return nullptr == current_ ?
             false : (nullptr == current_->previous_ ?
                      false : (nullptr != current_->next_));
    }

   private:

    BinodeBase *current_ = nullptr;

  };

  /**
   * @brief Default constructor.
   */
  CountedDeque() = default;

  explicit CountedDeque(const DeleterType &deleter)
      : CountedDequeBase(deleter) {}

  ~CountedDeque() override { clear(); }

  T *operator[](int index) const { return at(index); }

  T *at(int index) const;

  void push_front(T *obj);

  void push_back(T *obj);

  void insert(T *obj, int index = 0);

  size_t count() const { return count_; }

  bool is_empty() const { return 0 == count_; }

  void clear();

  void clear(const DeleterType &deleter);

  Iterator begin() const {
    if (head_.next_ == &tail_) {
      _ASSERT(tail_.previous_ == &head_);
      return Iterator();
    }

    const BinodeBase *p = head_.next_;
    return Iterator(const_cast<BinodeBase *>(p));
  }

  Iterator end() const {
    const BinodeBase *p = &tail_;
    return Iterator(const_cast<BinodeBase *>(p));
  }

};

template<typename T>
void CountedDequeNode<T>::unlink() {
  CountedDequeNodeBase::Unlink(this);
  _ASSERT(deque_ == nullptr);
}

template<typename T>
bool CountedDequeNode<T>::is_linked() const {
  bool ret = CountedDequeNodeBase::IsLinked(this);

  if (ret) {
    _ASSERT(nullptr != deque_);
  }

  return ret;
}

template<typename T>
void CountedDeque<T>::clear() {
  CountedDequeNodeBase *tmp = nullptr;
  while (!is_empty()) {
    _ASSERT(head_.next_ != &tail_);
    _ASSERT(tail_.previous_ != &head_);

    tmp = static_cast<internal::CountedDequeNodeTraits *>(head_.next_)->node_;
    CountedDequeNodeBase::Unlink(tmp);
    _ASSERT(tmp->deque_ == nullptr);
    if (deleter_) deleter_(tmp);
  }

  _ASSERT(count_ == 0);
//    count_ = 0;
}

template<typename T>
void CountedDeque<T>::clear(const DeleterType &deleter) {
  CountedDequeNodeBase *tmp = nullptr;
  while (!is_empty()) {
    _ASSERT(head_.next_ != &tail_);
    _ASSERT(tail_.previous_ != &head_);

    tmp = static_cast<internal::CountedDequeNodeTraits *>(head_.next_)->node_;
    CountedDequeNodeBase::Unlink(tmp);
    _ASSERT(tmp->deque_ == nullptr);
    if (deleter) deleter(tmp);
  }

  _ASSERT(count_ == 0);
//  count_ = 0;
}

template<typename T>
T *CountedDeque<T>::at(int index) const {
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

  return static_cast<T *>(static_cast<internal::CountedDequeNodeTraits *>(p)->node_);
}

template<typename T>
void CountedDeque<T>::push_front(T *obj) {
  BinodeBase::PushBack(&head_, &obj->traits_);
  obj->deque_ = this;
  ++count_;
}

template<typename T>
void CountedDeque<T>::push_back(T *obj) {
  BinodeBase::PushFront(&tail_, &obj->traits_);
  obj->deque_ = this;
  ++count_;
}

template<typename T>
void CountedDeque<T>::insert(T *obj, int index) {
  CountedDequeNodeBase::Unlink(obj);

  if (index >= 0) {
    BinodeBase *p = head_.next_;
    while ((&tail_ != p) && (index > 0)) {
      p = p->next_;
      index--;
    }
    BinodeBase::PushFront(p, &obj->traits_);
  } else {
    BinodeBase *p = tail_.previous_;
    while ((&head_ != p) && (index < -1)) {
      p = p->previous_;
      index++;
    }
    BinodeBase::PushBack(p, &obj->traits_);
  }

  obj->deque_ = this;
  count_++;
}

} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_COMPOUND_DEQUE_HPP_
