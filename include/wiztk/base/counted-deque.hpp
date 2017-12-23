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
class CountedDequeBase;

/**
 * @ingroup base
 * @brief The basic class for bidirectional node with pointer to a countable deque.
 */
class CountedDequeNodeBase : public DequeNodeBase {

  template<typename T> friend
  class CountedDequeNode;

  friend class CountedDequeBase;

  template<typename T> friend
  class CountedDeque;

 public:

  /**
   * @brief Destructor.
   *
   * This will decrease the count in deque.
   */
  ~CountedDequeNodeBase() override;

 protected:

  /**
   * @brief A pointer to a fast countable deque object.
   */
  CountedDequeBase *deque_ = nullptr;

 private:

  CountedDequeNodeBase() = default;

};

/**
 * @ingroup base
 * @brief Template class for bidirectional node used in fast countable deque.
 * @tparam T Must be a subclass of CountedBinodeBase.
 */
template<typename T>
class CountedDequeNode : public CountedDequeNodeBase {

  template<typename R> friend
  class CountedDeque;

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

  T *previous() const { return dynamic_cast<T *>(previous_); }

  T *next() const { return dynamic_cast<T *>(next_); }

  CountedDequeBase *deque() const { return deque_; }

};

/**
 * @ingroup base
 * @brief Counted deque base
 */
class CountedDequeBase {

  friend class CountedDequeNodeBase;

  template<typename T> friend
  class CountedDequeNode;

  template<typename T> friend
  class CountedDeque;

 public:

  typedef std::function<void(BinodeBase *)> DeleterType;

  virtual ~CountedDequeBase();

 protected:

  Deque<CountedDequeNodeBase> deque_;

  size_t count_ = 0;

 private:

  CountedDequeBase() = default;

  explicit CountedDequeBase(const DeleterType &deleter)
      : deque_(deleter) {}

};

/**
 * @ingroup base
 * @brief Counted deque.
 * @tparam T
 */
template<typename T>
class CountedDeque : public CountedDequeBase {

 public:

  /**
    * @brief A nested iterator for CountedDeque.
    */
  class Iterator {

   public:

    explicit Iterator(T *obj = nullptr)
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

    T *get() const noexcept {
      return nullptr == current_->previous_ ?
             nullptr : (nullptr == current_->next_ ?
                        nullptr : current_);
    }

    explicit operator bool() const {
      return nullptr == current_ ?
             false : (nullptr == current_->previous_ ?
                      false : (nullptr != current_->next_));
    }

   private:

    T *current_;

  };

  CountedDeque() = default;

  explicit CountedDeque(const DeleterType &deleter)
      : CountedDequeBase(deleter) {}

  ~CountedDeque() override = default;

  T *at(int index) const { return static_cast<T *>(deque_.at(index)); }

  void push_back(T *obj) {
    obj->unlink();
    deque_.push_back(obj);
    obj->deque_ = this;
    count_++;
  }

  void push_front(T *obj) {
    obj->unlink();
    deque_.push_front(obj);
    obj->deque_ = this;
    count_++;
  }

  void insert(T *obj, int index = 0) {
    obj->unlink();
    deque_.insert(obj, index);
    obj->deque_ = this;
    count_++;
  }

  void clear() {
    CountedDequeNodeBase *tmp = nullptr;
    while (!deque_.is_empty()) {
      tmp = deque_.begin().get();
      BinodeBase::Unlink(tmp);
      tmp->deque_ = nullptr;
      if (deque_.deleter_) deque_.deleter_(tmp);
    }

    count_ = 0;
  }

  void clear(const DeleterType &deleter) {
    CountedDequeNodeBase *tmp = nullptr;
    while (!deque_.is_empty()) {
      tmp = deque_.begin().get();
      BinodeBase::Unlink(tmp);
      tmp->deque_ = nullptr;
      if (deleter) deleter(tmp);
    }

    count_ = 0;
  }

  size_t count() const { return count_; }

};

template<typename T>
void CountedDequeNode<T>::unlink() {
  Unlink(this);

  if (nullptr != deque_) {
    _ASSERT(deque_->count_ > 0);
    deque_->count_--;
    deque_ = nullptr;
  }
}

template<typename T>
bool CountedDequeNode<T>::is_linked() const {
  bool ret = IsLinked(this);

  if (ret) {
    _ASSERT(nullptr != deque_);
  }

  return ret;
}

} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_COMPOUND_DEQUE_HPP_
