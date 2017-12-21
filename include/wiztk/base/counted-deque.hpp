/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
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

#ifndef WIZTK_BASE_COUNTED_DEQUE_HPP_
#define WIZTK_BASE_COUNTED_DEQUE_HPP_

#include "wiztk/base/macros.hpp"

#include "wiztk/base/binode.hpp"
#include "wiztk/base/deque.hpp"

namespace wiztk {
namespace base {

// Foward declaration:
class CountedDequeBase;

class CountedBinodeBase : public BinodeBase {

  friend class CountedDequeBase;

  template<typename T> friend
  class CountedBinode;

  template<typename T> friend
  class CountedDequeExt;

 public:

  ~CountedBinodeBase() override;

 protected:

  CountedDequeBase *deque_ = nullptr;

 private:

  CountedBinodeBase() = default;

};

template<typename T>
class CountedBinode : protected CountedBinodeBase {

  template<typename R> friend
  class CountedDequeExt;

 public:

  CountedBinode() = default;

  ~CountedBinode() override = default;

  void unlink();

  bool is_linked() const;

  T *previous() const { return static_cast<T *>(previous_); }

  T *next() const { return static_cast<T *>(next_); }

};

class CountedDequeBase : public Deque<CountedBinodeBase> {

  friend class CountedBinodeBase;

  template<typename T> friend
  class CountedBinode;

  template<typename T> friend
  class CountedDequeExt;

 public:

  ~CountedDequeBase() override;

 protected:

  size_t count_ = 0;

 private:

  CountedDequeBase() = default;

  explicit CountedDequeBase(const DeleterType &deleter)
      : Deque<CountedBinodeBase>(deleter) {}

};

template<typename T>
class CountedDequeExt : protected CountedDequeBase {

 public:

  CountedDequeExt() = default;

  explicit CountedDequeExt(const DeleterType &deleter)
      : CountedDequeBase(deleter) {}

  ~CountedDequeExt() override = default;

  void push_back(T *obj) {
    obj->unlink();
    PushBack(obj);
    obj->deque_ = this;
    count_++;
  }

  void push_front(T *obj) {
    obj->unlink();
    PushFront(obj);
    obj->deque_ = this;
    count_++;
  }

  void insert(T *obj, int index = 0) {
    obj->unlink();
    Insert(obj, index);
    obj->deque_ = this;
    count_++;
  }

  void clear() { Clear(); }

  void clear(const DeleterType &deleter) { Clear(deleter); }

  size_t count() const { return count_; }

};

template<typename T>
void CountedBinode<T>::unlink() {
  Unlink();

  if (nullptr != deque_) {
    _ASSERT(deque_->count_ > 0);
    deque_->count_--;
    deque_ = nullptr;
  }
}

template<typename T>
bool CountedBinode<T>::is_linked() const {
  bool ret = IsLinked();

  if (ret) {
    _ASSERT(nullptr != deque_);
  }

  return ret;
}

// ------

/**
 * @ingroup base
 * @brief A double-ended queue container with an element counter.
 */
class CountedDeque {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(CountedDeque);

  /**
 * @brief A nested class represents an element in a deque
 */
  class Element {

    friend class CountedDeque;

   public:

    WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Element);

    Element() = default;

    virtual ~Element();

    Element *previous() const { return previous_; }

    Element *next() const { return next_; }

    CountedDeque *deque() const { return deque_; }

   private:

    Element *previous_ = nullptr;
    Element *next_ = nullptr;
    CountedDeque *deque_ = nullptr;

  };

  /**
   * @brief A nested iterator for deque
   */
  class Iterator {

   public:

    explicit Iterator(Element *element = nullptr)
        : element_(element) {}

    Iterator(const Iterator &orig) = default;

    ~Iterator() = default;

    Iterator &operator=(const Iterator &other) = default;

    Iterator &operator++() {
      element_ = element_->next_;
      return *this;
    }

    Iterator operator++(int) {
      Iterator retval;
      retval.element_ = element_->next_;
      return retval;
    }

    Iterator &operator--() {
      element_ = element_->previous_;
      return *this;
    }

    Iterator operator--(int) {
      Iterator retval;
      retval.element_ = element_->previous_;
      return retval;
    }

    bool operator==(const Iterator &other) const {
      return element_ == other.element_;
    }

    bool operator==(const Element *element) const {
      return element_ == element;
    }

    bool operator!=(const Iterator &other) const {
      return element_ != other.element_;
    }

    bool operator!=(const Element *element) const {
      return element_ != element;
    }

    template<typename T>
    T *cast() const {
      return static_cast<T *>(element_);
    }

    explicit operator bool() const {
      return nullptr != element_;
    }

   private:

    Element *element_;

  };

  CountedDeque() = default;

  virtual ~CountedDeque();

  void PushFront(Element *item);

  void PushBack(Element *item);

  void Insert(Element *item, int index = 0);

  Element *Remove(Element *item);

  void Clear();

  Element *operator[](int index) const;

  Iterator begin() const {
    return Iterator(first_);
  }

  Iterator rbegin() const {
    return Iterator(last_);
  }

  Iterator end() const {
    return Iterator(last_->next_);
  }

  Iterator rend() const {
    return Iterator(first_->previous_);
  }

  int count() const { return count_; }

  Element *first() const { return first_; }

  Element *last() const { return last_; }

 private:

  Element *first_ = nullptr;
  Element *last_ = nullptr;
  int count_ = 0;

};

} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_COMPOUND_DEQUE_HPP_
