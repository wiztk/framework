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

#include <wiztk/base/object.hpp>

namespace wiztk {
namespace base {

Object::Object()
    : Trackable(),
      previous_(nullptr),
      next_(nullptr),
      first_child_(nullptr),
      last_child_(nullptr),
      parent_(nullptr),
      children_count_(0) {
}

Object::~Object() {
  if (parent_) parent_->RemoveChild(this);
  _ASSERT(previous_ == nullptr);
  _ASSERT(next_ == nullptr);

  if (children_count_ > 0) {
    _ASSERT(first_child_ != nullptr);
    _ASSERT(last_child_ != nullptr);
    ClearChildren();
  }
  _ASSERT(children_count_ == 0);
  _ASSERT(first_child_ == nullptr);
  _ASSERT(last_child_ == nullptr);
}

Object *Object::GetChildAt(int index) const {
  if (index < 0) index = children_count_ + index;

  Object *object = nullptr;
  int mid = (children_count_ + 1) / 2;

  if (index >= 0 && index < mid) {
    object = first_child_;
    while (index > 0) {
      object = object->next_;
      index--;
    }
  } else if (index >= mid && index < children_count_) {
    object = last_child_;
    int max = children_count_ - 1;
    while (index < max) {
      object = object->previous_;
      index++;
    }
  }

  return object;
}

void Object::PushFrontChild(Object *child) {
  if (child->parent_) {
    if (child->parent_ == this) {
      return;
    } else {
      child->parent_->RemoveChild(child);
    }
  }

  _ASSERT(child->previous_ == nullptr);
  _ASSERT(child->next_ == nullptr);
  _ASSERT(child->parent_ == nullptr);

  if (first_child_) {
    first_child_->previous_ = child;
    child->next_ = first_child_;
  } else {
    _ASSERT(last_child_ == nullptr);
    _ASSERT(children_count_ == 0);
    // child->next_ = nullptr;
    last_child_ = child;
  }
  first_child_ = child;

  // child->previous_ = nullptr;
  child->parent_ = this;
  children_count_++;

  child->OnAddedToParent();
}

void Object::InsertChild(Object *child, int index) {
  if (child->parent_) {
    if (child->parent_ == this) {
      return;
    } else {
      child->parent_->RemoveChild(child);
    }
  }

  _ASSERT(child->previous_ == nullptr);
  _ASSERT(child->next_ == nullptr);
  _ASSERT(child->parent_ == nullptr);

  if (first_child_ == nullptr) {
    _ASSERT(last_child_ == nullptr);
    _ASSERT(children_count_ == 0);
    // child->next_ = nullptr;
    // child->previous_ = nullptr;
    last_child_ = child;
    first_child_ = child;
  } else {
    if (index >= 0) {
      Object *p = first_child_;
      while (p && (index > 0)) {
        p = p->next_;
        index--;
      }
      if (p) {  // insert before p
        child->previous_ = p->previous_;
        child->next_ = p;
        if (p->previous_) p->previous_->next_ = child;
        else first_child_ = child;
        p->previous_ = child;
      } else {  // push back
        last_child_->next_ = child;
        child->previous_ = last_child_;
        last_child_ = child;
        // child->next_ = nullptr;
      }
    } else {
      Object *p = last_child_;
      while (p && (index < -1)) {
        p = p->previous_;
        index++;
      }
      if (p) {  // insert after p
        child->next_ = p->next_;
        child->previous_ = p;
        if (p->next_) p->next_->previous_ = child;
        else last_child_ = child;
        p->next_ = child;
      } else {  // push front
        first_child_->previous_ = child;
        child->next_ = first_child_;
        first_child_ = child;
        // child->previous_ = nullptr;
      }
    }
  }

  child->parent_ = this;
  children_count_++;

  child->OnAddedToParent();
}

void Object::PushBackChild(Object *child) {
  if (child->parent_) {
    if (child->parent_ == this) {
      return;
    } else {
      child->parent_->RemoveChild(child);
    }
  }

  _ASSERT(child->previous_ == nullptr);
  _ASSERT(child->next_ == nullptr);
  _ASSERT(child->parent_ == nullptr);

  if (last_child_) {
    last_child_->next_ = child;
    child->previous_ = last_child_;
  } else {
    _ASSERT(first_child_ == nullptr);
    _ASSERT(children_count_ == 0);
    // child->previous_ = nullptr;
    first_child_ = child;
  }
  last_child_ = child;

  // child->next_ = nullptr;
  child->parent_ = this;
  children_count_++;

  child->OnAddedToParent();
}

Object *Object::RemoveChild(Object *child) {
  if (child->parent_ != this) return nullptr;

  _ASSERT(children_count_ > 0);

  if (child->previous_) {
    child->previous_->next_ = child->next_;
  } else {
    _ASSERT(first_child_ == child);
    first_child_ = child->next_;
  }

  if (child->next_) {
    child->next_->previous_ = child->previous_;
  } else {
    _ASSERT(last_child_ == child);
    last_child_ = child->previous_;
  }

  children_count_--;
  _ASSERT(children_count_ >= 0);

  child->previous_ = nullptr;
  child->next_ = nullptr;
  child->parent_ = nullptr;

  child->OnRemovedFromParent(this);

  return child;
}

void Object::ClearChildren() {
  Object *ptr = first_child_;
  Object *next_ptr = nullptr;

  while (ptr) {
    next_ptr = ptr->next_;
    // ptr->previous_ = nullptr;
    // ptr->next_ = nullptr;
    // ptr->parent_ = nullptr;
    delete ptr;
    ptr = next_ptr;
  }

  children_count_ = 0;
  first_child_ = nullptr;
  last_child_ = nullptr;
}

void Object::OnAddedToParent() {
  // override in subclass
}

void Object::OnRemovedFromParent(Object *original_parent) {
  // override in subclass
}

bool Object::SwapIndex(Object *object1, Object *object2) {
  if (object1 == nullptr || object2 == nullptr) return false;
  if (object1 == object2) return false;
  if (object1->parent_ != object2->parent_) return false;
  if (object1->parent_ == nullptr) return false;

  Object *tmp1 = nullptr;
  Object *tmp2 = nullptr;

  if (object1->next_ == object2) {    // view1 is just the previous sibling of view2

    _ASSERT(object2->previous_ == object1);
    tmp1 = object1->previous_;
    tmp2 = object2->next_;
    object2->previous_ = tmp1;
    object1->previous_ = object2;
    object2->next_ = object1;
    object1->next_ = tmp2;

    if (tmp1 != nullptr) tmp1->next_ = object2;
    else object1->parent_->first_child_ = object2;

    if (tmp2 != nullptr) tmp2->previous_ = object1;
    else object2->parent_->last_child_ = object2;

  } else if (object1->previous_ == object2) {
    _ASSERT(object2->next_ == object1);

    tmp1 = object2->previous_;
    tmp2 = object1->next_;
    object1->previous_ = tmp1;
    object2->previous_ = object1;
    object1->next_ = object2;
    object2->next_ = tmp2;

    if (tmp1 != nullptr) tmp1->next_ = object1;
    else object2->parent_->first_child_ = object1;

    if (tmp2 != nullptr) tmp2->previous_ = object2;
    else object1->parent_->last_child_ = object2;

  } else {

    tmp1 = object1->previous_;
    tmp2 = object2->previous_;
    object1->previous_ = tmp2;
    object2->previous_ = tmp1;

    if (tmp1 != nullptr) tmp1->next_ = object2;
    else object1->parent_->first_child_ = object2;

    if (tmp2 != nullptr) tmp2->next_ = object1;
    else object2->parent_->first_child_ = object1;

    tmp1 = object1->next_;
    tmp2 = object2->next_;
    object1->next_ = tmp2;
    object2->next_ = tmp1;

    if (tmp1 != nullptr) tmp1->previous_ = object2;
    else object1->parent_->last_child_ = object2;

    if (tmp2 != nullptr) tmp2->previous_ = object1;
    else object2->parent_->last_child_ = object1;

  }

  return true;
}

bool Object::InsertSiblingBefore(Object *src, Object *dst) {
  if (src == nullptr || dst == nullptr) return false;
  if (src == dst) return false;

  if (dst->parent_ != nullptr) {

    if (dst->parent_ == src->parent_) {

      if (src->previous_ == dst) {    // already is the previous one of src
        return true;
      }

      if (dst->previous_) {
        dst->previous_->next_ = dst->next_;
      } else {
        _ASSERT(dst->parent_->first_child_ == dst);
        dst->parent_->first_child_ = dst->next_;
      }

      if (dst->next_) {
        dst->next_->previous_ = dst->previous_;
      } else {
        _ASSERT(dst->parent_->last_child_ == dst);
        dst->parent_->last_child_ = dst->previous_;
      }

      Object *tmp = src->previous_;

      src->previous_ = dst;
      dst->next_ = src;
      dst->previous_ = tmp;

      if (tmp) {
        tmp->next_ = dst;
      } else {
        _ASSERT(src->parent_->first_child_ == src);
        dst->parent_->first_child_ = dst;
      }

      return true;

    } else {
      dst->parent_->RemoveChild(dst);
    }

  }
  _ASSERT(dst->parent_ == nullptr);
  _ASSERT(dst->next_ == nullptr);
  _ASSERT(dst->previous_ == nullptr);

  Object *tmp = src->previous_;

  src->previous_ = dst;
  dst->next_ = src;
  dst->previous_ = tmp;
  if (tmp) {
    tmp->next_ = dst;
  } else {
    _ASSERT(src->parent_->first_child_ == src);
    src->parent_->first_child_ = dst;
  }

  dst->parent_ = src->parent_;
  src->parent_->children_count_++;

  return true;
}

bool Object::InsertSiblingAfter(Object *src, Object *dst) {
  if (src == nullptr || dst == nullptr) return false;
  if (src == dst) return false;

  if (dst->parent_ != nullptr) {

    if (dst->previous_ == src->parent_) {

      if (src->next_ == dst) {    // alrady is the next one of src
        return true;
      }

      if (dst->previous_) {
        dst->previous_->next_ = dst->next_;
      } else {
        _ASSERT(dst->parent_->first_child_ == dst);
        dst->parent_->first_child_ = dst->next_;
      }

      if (dst->next_) {
        dst->next_->previous_ = dst->previous_;
      } else {
        _ASSERT(dst->parent_->last_child_ == dst);
        dst->parent_->last_child_ = dst->previous_;
      }

      Object *tmp = src->next_;

      src->next_ = dst;
      dst->previous_ = src;
      dst->next_ = tmp;

      if (tmp) {
        tmp->previous_ = dst;
      } else {
        _ASSERT(src->parent_->last_child_ == src);
        dst->parent_->last_child_ = dst;
      }

      return true;

    } else {
      dst->parent_->RemoveChild(dst);
    }

  }

  _ASSERT(dst->parent_ == nullptr);
  _ASSERT(dst->next_ == nullptr);
  _ASSERT(dst->previous_ == nullptr);

  Object *tmp = src->next_;

  src->next_ = dst;
  dst->previous_ = src;
  dst->next_ = tmp;
  if (tmp) {
    tmp->previous_ = dst;
  } else {
    _ASSERT(src->parent_->last_child_ == src);
    src->parent_->last_child_ = dst;
  }

  dst->parent_ = src->parent_;
  src->parent_->children_count_++;

  return true;
}

void Object::MoveToFirst(Object *object) {
  if (object->parent_) {

    if (object->parent_->first_child_ == object) {
      _ASSERT(object->previous_ == 0);
      return;    // already at first
    }

    object->previous_->next_ = object->next_;
    if (object->next_) {
      object->next_->previous_ = object->previous_;
    } else {
      _ASSERT(object->parent_->last_child_ == object);
      object->parent_->last_child_ = object->previous_;
    }

    object->previous_ = 0;
    object->next_ = object->parent_->first_child_;
    object->parent_->first_child_->previous_ = object;
    object->parent_->first_child_ = object;
  }
}

void Object::MoveToLast(Object *object) {
  if (object->parent_) {

    if (object->parent_->last_child_ == object) {
      _ASSERT(object->next_ == 0);
      return;    // already at last
    }

    object->next_->previous_ = object->previous_;

    if (object->previous_) {
      object->previous_->next_ = object->next_;
    } else {
      _ASSERT(object->parent_->first_child_ == object);
      object->parent_->first_child_ = object->next_;
    }

    object->next_ = 0;
    object->previous_ = object->parent_->last_child_;
    object->parent_->last_child_->next_ = object;
    object->parent_->last_child_ = object;
  }
}

void Object::MoveForward(Object *object) {
  if (object->parent_) {

    if (object->next_) {

      Object *tmp = object->next_;

      tmp->previous_ = object->previous_;
      if (object->previous_) {
        object->previous_->next_ = tmp;
      } else {
        _ASSERT(object->parent_->first_child_ == object);
        object->parent_->first_child_ = tmp;
      }

      object->previous_ = tmp;
      object->next_ = tmp->next_;
      if (tmp->next_) {
        tmp->next_->previous_ = object;
      }
      tmp->next_ = object;

      if (object->next_ == 0) {
        _ASSERT(object->parent_->last_child_ == tmp);
        object->parent_->last_child_ = object;
      }

      if (object->previous_) {
        _ASSERT(object->previous_->next_ == object);
      }
      if (object->next_) {
        _ASSERT(object->next_->previous_ == object);
      }

    } else {
      _ASSERT(object->parent_->last_child_ == object);
    }

  }
}

void Object::MoveBackward(Object *object) {
  if (object->parent_) {

    if (object->previous_) {

      Object *tmp = object->previous_;

      tmp->next_ = object->next_;
      if (object->next_) {
        object->next_->previous_ = tmp;
      } else {
        _ASSERT(object->parent_->last_child_ == object);
        object->parent_->last_child_ = tmp;
      }

      object->next_ = tmp;
      object->previous_ = tmp->previous_;
      if (tmp->previous_) {
        tmp->previous_->next_ = object;
      }
      tmp->previous_ = object;

      if (object->previous_ == 0) {
        _ASSERT(object->parent_->first_child_ == tmp);
        object->parent_->first_child_ = object;
      }

      if (object->previous_) {
        _ASSERT(object->previous_->next_ == object);
      }
      if (object->next_) {
        _ASSERT(object->next_->previous_ == object);
      }

    } else {
      _ASSERT(object->parent_->first_child_ == object);
    }

  }
}

} // namespace base
} // namespace wiz-vision
