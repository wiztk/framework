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

#ifndef WIZTK_CORE_DEQUE_HPP_
#define WIZTK_CORE_DEQUE_HPP_

#include "macros.hpp"

#include <cstddef>

namespace wiztk {
namespace base {

/**
 * @ingroup base
 * @brief A bidirectional node used in deque or custom list
 *
 * You usually don't use this class directly. Instead, you use or create a subclass.
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
class BiNode {

  template<typename T> friend
  class DequeT;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(BiNode);

  /**
   * @brief Default constructor
   */
  BiNode() = default;

  /**
   * @brief Constructor
   */
  virtual ~BiNode();

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
  void PushFront(BiNode *other);

  /**
   * @brief Push another node to the back
   * @param other
   */
  void PushBack(BiNode *other);

  /**
   * @brief Break the link to both the previous and next node
   */
  void Unlink();

  /**
   * @brief Getter of previous node
   * @return
   */
  BiNode *previous() const { return previous_; }

  /**
   * @brief Getter of next node
   * @return
   */
  BiNode *next() const { return next_; }

 private:

  BiNode *previous_ = nullptr;
  BiNode *next_ = nullptr;

};

/**
 * @ingroup base
 * @brief A simple double-ended queue container
 * @tparam T Must be a BiNode class or subclass
 */
template<typename T = BiNode>
class DequeT {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(DequeT);

  /**
   * @brief A nested iterator for deque
   */
  class Iterator {

   public:

    explicit Iterator(BiNode *element = nullptr)
        : element_(element) {}

    Iterator(const Iterator &orig) = default;

    ~Iterator() = default;

    Iterator &operator=(const Iterator &other) = default;

    Iterator &operator++() {
      element_ = element_->next_;
      return *this;
    }

    Iterator operator++(int) {
      return Iterator(element_->next_);
    }

    Iterator &operator--() {
      element_ = element_->previous_;
      return *this;
    }

    Iterator operator--(int) {
      return Iterator(element_->previous_);
    }

    void PushFront(T *element) {
      element_->PushFront(element);
    }

    void PushBack(T *element) {
      element_->PushBack(element);
    }

    /**
     * @brief Unlink the bi-node object and invalidate this iterator
     *
     * @note Once this method is called, this iterator is invalidated and
     * should not be used, but it can be assigned to another one.
     */
    void Remove() {
      element_->Unlink();
      element_ = nullptr;
    }

    bool operator==(const Iterator &other) const {
      return element_ == other.element_;
    }

    bool operator==(const T *element) const {
      return element_ == element;
    }

    bool operator!=(const Iterator &other) const {
      return element_ != other.element_;
    }

    bool operator!=(const T *element) const {
      return element_ != element;
    }

    T *element() const {
      return static_cast<T *>(element_);
    }

    explicit operator bool() const {
      return nullptr == element_ ?
             false : (nullptr == element_->previous_ ?
                      false : (nullptr != element_->next_));
    }

   private:

    BiNode *element_;

  };

  /**
   * @brief A nested const iterator class
   */
  class ConstIterator {

   public:

    ConstIterator() = delete;

    explicit ConstIterator(const BiNode *element = nullptr)
        : element_(element) {}

    ConstIterator(const ConstIterator &orig) = default;

    ~ConstIterator() = default;

    ConstIterator &operator=(const ConstIterator &other) = default;

    ConstIterator &operator++() {
      element_ = element_->next_;
      return *this;
    }

    ConstIterator operator++(int) {
      return ConstIterator(element_->next_);
    }

    ConstIterator &operator--() {
      element_ = element_->previous_;
      return *this;
    }

    ConstIterator operator--(int) {
      return ConstIterator(element_->previous_);
    }

    bool operator==(const ConstIterator &other) const {
      return element_ == other.element_;
    }

    bool operator==(const T *element) const {
      return element_ == element;
    }

    bool operator!=(const ConstIterator &other) const {
      return element_ != other.element_;
    }

    bool operator!=(const T *element) const {
      return element_ != element;
    }

    const T *element() const {
      return nullptr == element_->previous_ ?
             nullptr : (nullptr == element_->next_ ?
                        nullptr : (static_cast<const T *>(element_)));
    }

    explicit operator bool() const {
      return nullptr == element_ ?
             false : (nullptr == element_->previous_ ?
                      false : (nullptr != element_->next_));
    }

   private:

    const BiNode *element_;

  };

  DequeT();

  virtual ~DequeT();

  void PushFront(T *item);

  void PushBack(T *item);

  void Insert(T *item, int index = 0);

  T *Remove(T *item);

  size_t GetSize() const;

  bool IsEmpty() const;

  void Clear();

  T *operator[](int index) const;

  Iterator begin() const {
    return Iterator(first_.next_);
  }

  ConstIterator cbegin() const {
    return ConstIterator(first_.next_);
  }

  Iterator rbegin() const {
    return Iterator(last_.previous_);
  }

  ConstIterator crbegin() const {
    return ConstIterator(last_.previous_);
  }

  Iterator end() const {
    return Iterator(const_cast<BiNode *>(&last_));
  }

  ConstIterator cend() const {
    return ConstIterator(&last_);
  }

  Iterator rend() const {
    return Iterator(const_cast<BiNode *>(&first_));
  }

  ConstIterator crend() const {
    return ConstIterator(&first_);
  }

 protected:

  const BiNode *first() const { return &first_; }

  const BiNode *last() const { return &last_; }

 private:

  BiNode first_;
  BiNode last_;

};

template<typename T>
DequeT<T>::DequeT() {
  first_.next_ = &last_;
  last_.previous_ = &first_;
}

template<typename T>
DequeT<T>::~DequeT() {
  Clear();
}

template<typename T>
void DequeT<T>::PushFront(T *item) {
  item->Unlink();
  first_.PushBack(item);
}

template<typename T>
void DequeT<T>::PushBack(T *item) {
  item->Unlink();
  last_.PushFront(item);
}

template<typename T>
void DequeT<T>::Insert(T *item, int index) {
  if (index >= 0) {
    T *p = first_.next_;
    while ((&last_ != p) && (index > 0)) {
      p = p->next_;
      index--;
    }
    p->PushFront(item);
  } else {
    T *p = last_.previous_;
    while ((&first_ != p) && (index < -1)) {
      p = p->previous_;
      index++;
    }
    p->PushBack(item);
  }
}

template<typename T>
T *DequeT<T>::Remove(T *item) {
  if (nullptr == item) return nullptr;

  for (Iterator it = begin(); it != end(); ++it) {
    if (it == item) {
      it.Remove();
      return item;
    }
  }

  return nullptr;
}

template<typename T>
size_t DequeT<T>::GetSize() const {
  size_t size = 0;

  BiNode *element = first_.next_;
  while (element != &last_) {
    ++size;
    element = element->next_;
  }

  return size;
}

template<typename T>
bool DequeT<T>::IsEmpty() const {
  return first_.next_ == &last_;
}

template<typename T>
void DequeT<T>::Clear() {
  while (first_.next_ != &last_) {
    first_.next_->Unlink();
  }
}

template<typename T>
T *DequeT<T>::operator[](int index) const {
  BiNode *p = nullptr;

  if (index >= 0) {
    p = first_.next_;
    while ((&last_ != p) && (index > 0)) {
      p = p->next_;
      index--;
    }
  } else {
    p = last_.previous_;
    while ((&first_ != p) && (index < -1)) {
      p = p->previous_;
      index++;
    }
  }

  return static_cast<T *>(p);
}

} // namespace base
} // namespace wiztk

#endif // WIZTK_CORE_DEQUE_HPP_
