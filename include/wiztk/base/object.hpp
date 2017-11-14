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

#ifndef WIZTK_CORE_OBJECT_HPP_
#define WIZTK_CORE_OBJECT_HPP_

#include "sigcxx.hpp"
#include "macros.hpp"

#include <string>

namespace wiztk {
namespace base {

/**
 * @ingroup base
 * @brief A trackable object class with ability to build object trees.
 *
 * Object is the basic class in this project. It's usually for building GUI
 * object trees.
 *
 * Every Object has a parent (and only one parent at the same time) and
 * children. When an object is destroyed, all children will be destroyed too.
 */
class Object : public Trackable {

  Object(const Object &other) = delete;
  Object &operator=(const Object &other) = delete;

 public:

  Object();

  virtual ~Object();

  Object *parent() const { return parent_; }

 protected:

  Object *previous() const { return previous_; }

  Object *next() const { return next_; }

  Object *first_child() const { return first_child_; }

  Object *last_child() const { return last_child_; }

  int children_count() const { return children_count_; }

  Object *GetChildAt(int index) const;

  /**
   * @brief Push a child object to the front
   * @param child
   *
   * @warning This method does not check if child is nullptr
   */
  void PushFrontChild(Object *child);

  /**
   * @brief Insert a child object at the given index
   * @param child
   * @param index The position to be inserted before, default is 0, same as push front
   *
   * @warning This method does not check if child is nullptr
   */
  void InsertChild(Object *child, int index = 0);

  /**
   * @brief Push a child object to the back
   * @param child
   *
   * @warning This method does not check if child is nullptr
   */
  void PushBackChild(Object *child);

  /**
   * @brief Remove a child object from the children list
   * @param child
   * @return nullptr if the object is not a child, or the removed object
   *
   * @warning This method does not check if the param is nullptr
   */
  Object *RemoveChild(Object *child);

  void ClearChildren();

  virtual void OnAddedToParent();

  virtual void OnRemovedFromParent(Object *original_parent);

  static bool SwapIndex(Object *object1, Object *object2);

  static bool InsertSiblingBefore(Object *src, Object *dst);

  static bool InsertSiblingAfter(Object *src, Object *dst);

  static void MoveToFirst(Object *object);

  static void MoveToLast(Object *object);

  static void MoveForward(Object *object);

  static void MoveBackward(Object *object);

  template<typename T>
  static T *GetManagedObjectAt(int index,
                               T *first,
                               T *last,
                               int count);

  template<typename M, typename T>
  static void PushFrontManagedObject(M *_this,
                                     T *object,
                                     M **manager,
                                     T **first,
                                     T **last,
                                     int &count);

  template<typename M, typename T>
  static void InsertManagedObject(M *_this,
                                  T *object,
                                  M **manager,
                                  T **first,
                                  T **last,
                                  int &count,
                                  int index = 0);

  template<typename M, typename T>
  static void PushBackManagedObject(M *_this,
                                    T *object,
                                    M **manager,
                                    T **first,
                                    T **last,
                                    int &count);

  template<typename M, typename T>
  static T *RemoveManagedObject(M *_this,
                                T *object,
                                M **manager,
                                T **first,
                                T **last,
                                int &count);

  template<typename M, typename T>
  static void ClearManagedObject(M *_this,
                                 T **first,
                                 T **last,
                                 int &count);

 private:

  Object *previous_;
  Object *next_;

  Object *first_child_;
  Object *last_child_;

  Object *parent_;
  int children_count_;

};

template<typename T>
T *Object::GetManagedObjectAt(int index,
                              T *first,
                              T *last,
                              int count) {
  if (index >= count) return nullptr;

  Object *object = 0;
  if (index < ((count + 1) / 2)) {
    object = first;
    while (index > 0) {
      object = object->next_;
      index--;
    }
  } else {
    object = last;
    int max = count - 1;
    while (index < max) {
      object = object->previous_;
      index++;
    }
  }

  return static_cast<T *>(object);
}

template<typename M, typename T>
void Object::PushFrontManagedObject(M *_this,
                                    T *object,
                                    M **manager,
                                    T **first,
                                    T **last,
                                    int &count) {
  if (*manager) {
    _ASSERT(nullptr == object->parent_);
    if ((*manager) == _this) {
      return;
    } else {
      RemoveManagedObject(_this, object, manager, first, last, count);
    }
  } else if (object->parent_) {
    _ASSERT(nullptr == (*manager));
    object->parent_->RemoveChild(object);
  }

  _ASSERT(nullptr == object->previous_);
  _ASSERT(nullptr == object->next_);
  _ASSERT(nullptr == object->parent_);
  _ASSERT(nullptr == (*manager));

  if (*first) {
    (*first)->previous_ = object;
    object->next_ = (*first);
  } else {
    _ASSERT(nullptr == (*last));
    _ASSERT(0 == count);
    // object->next_ = nullptr;
    (*last) = object;
  }
  (*first) = object;

  // object->previous_ = nullptr;
  (*manager) = _this;
  count++;
}

template<typename M, typename T>
void Object::InsertManagedObject(M *_this,
                                 T *object,
                                 M **manager,
                                 T **first,
                                 T **last,
                                 int &count,
                                 int index) {
  if (*manager) {
    _ASSERT(nullptr == object->parent_);
    if ((*manager) == _this) {
      return;
    } else {
      RemoveManagedObject(_this, object, manager, first, last, count);
    }
  } else if (object->parent_) {
    object->parent_->RemoveChild(object);
  }

  _ASSERT(nullptr == object->previous_);
  _ASSERT(nullptr == object->next_);
  _ASSERT(nullptr == object->parent_);
  _ASSERT(nullptr == (*manager));

  if (nullptr == (*first)) {
    _ASSERT(nullptr == (*last));
    _ASSERT(0 == count);
    // object->next_ = nullptr;
    // object->previous_ = nullptr;
    (*last) = object;
    (*first) = object;
  } else {
    if (index >= 0) {
      Object *p = (*first);
      while (p && (index > 0)) {
        p = p->next_;
        index--;
      }
      if (p) {    // insert
        object->previous_ = p->previous_;
        object->next_ = p;
        if (p->previous_) p->previous_->next_ = object;
        else (*first) = object;
        p->previous_ = object;
      } else {    // push back
        (*last)->next_ = object;
        object->previous_ = (*last);
        (*last) = object;
        // object->next_ = nullptr;
      }
    } else {
      Object *p = (*last);
      while (p && (index < -1)) {
        p = p->previous_;
        index++;
      }
      if (p) {  // insert after p
        object->next_ = p->next_;
        object->previous_ = p;
        if (p->next_) p->next_->previous_ = object;
        else (*last) = object;
        p->next_ = object;
      } else {  // push front
        (*first)->previous_ = object;
        object->next_ = (*first);
        (*first) = object;
        // object->previous_ = nullptr;
      }

    }
  }

  (*manager) = _this;
  count++;
};

template<typename M, typename T>
void Object::PushBackManagedObject(M *_this,
                                   T *object,
                                   M **manager,
                                   T **first,
                                   T **last,
                                   int &count) {
  if (*manager) {
    _ASSERT(nullptr == object->parent_);
    if (_this == (*manager)) {
      return;
    } else {
      RemoveManagedObject(_this, object, manager, first, last, count);
    }
  } else if (object->parent_) {
    object->parent_->RemoveChild(object);
  }

  _ASSERT(nullptr == object->previous_);
  _ASSERT(nullptr == object->next_);
  _ASSERT(nullptr == object->parent_);
  _ASSERT(nullptr == (*manager));

  if (*last) {
    (*last)->next_ = object;
    object->previous_ = (*last);
  } else {
    _ASSERT(nullptr == (*first));
    _ASSERT(0 == count);
    // object->previous_ = nullptr;
    (*first) = object;
  }
  (*last) = object;

  // object->next_ = nullptr;
  (*manager) = _this;
  count++;
};

template<typename M, typename T>
T *Object::RemoveManagedObject(M *_this,
                               T *object,
                               M **manager,
                               T **first,
                               T **last,
                               int &count) {
  if ((*manager) != _this) return nullptr;

  _ASSERT(nullptr == object->parent_);
  _ASSERT(count > 0);

  if (object->previous_) {
    object->previous_->next_ = object->next_;
  } else {
    _ASSERT((*first) == object);
    (*first) = static_cast<T *>(object->next_);
  }

  if (object->next_) {
    object->next_->previous_ = object->previous_;
  } else {
    _ASSERT((*last) == object);
    (*last) = static_cast<T *>(object->previous_);
  }

  count--;
  _ASSERT(count >= 0);

  object->previous_ = nullptr;
  object->next_ = nullptr;
  (*manager) = nullptr;

  return object;
}

template<typename M, typename T>
void Object::ClearManagedObject(M *_this,
                                T **first,
                                T **last,
                                int &count) {
  Object *ptr = (*first);
  Object *next_ptr = nullptr;

  while (ptr) {
    next_ptr = ptr->next_;
    // ptr->previous_ = nullptr;
    // ptr->next_ = nullptr;
    // ptr->parent_ = nullptr;
    // ptr->MANAGER = nullptr;
    delete ptr;
    ptr = next_ptr;
  }

  count = 0;
  (*first) = nullptr;
  (*last) = nullptr;
}

} // namespace base
} // namespace wiztk

#endif // WIZTK_CORE_OBJECT_HPP_
