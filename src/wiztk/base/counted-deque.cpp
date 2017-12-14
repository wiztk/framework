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

#include "wiztk/base/counted-deque.hpp"

#include "wiztk/base/macros.hpp"

namespace wiztk {
namespace base {

CountedDeque::Element::~Element() {
  if (nullptr != deque_) deque_->Remove(this);
}

// -----

CountedDeque::~CountedDeque() {
  Clear();
}

void CountedDeque::PushFront(Element *item) {
  if (item->deque_ == this) return;

  if (nullptr != item->deque_) item->deque_->Remove(item);

  _ASSERT(nullptr == item->deque_);
  _ASSERT(nullptr == item->previous_);
  _ASSERT(nullptr == item->next_);

  if (nullptr != first_) {
    first_->previous_ = item;
    item->next_ = first_;
  } else {
    _ASSERT(nullptr == last_);
    _ASSERT(0 == count_);
    last_ = item;
  }
  first_ = item;

  item->deque_ = this;
  count_++;
}

void CountedDeque::PushBack(Element *item) {
  if (item->deque_ == this) return;

  if (nullptr != item->deque_) item->deque_->Remove(item);

  _ASSERT(nullptr == item->deque_);
  _ASSERT(nullptr == item->previous_);
  _ASSERT(nullptr == item->next_);

  if (nullptr != last_) {
    last_->next_ = item;
    item->previous_ = last_;
  } else {
    _ASSERT(nullptr == first_);
    _ASSERT(0 == count_);
    first_ = item;
  }
  last_ = item;

  item->deque_ = this;
  count_++;
}

void CountedDeque::Insert(Element *item, int index) {
  if (item->deque_ == this) return;

  if (nullptr != item->deque_) item->deque_->Remove(item);

  _ASSERT(nullptr == item->deque_);
  _ASSERT(nullptr == item->previous_);
  _ASSERT(nullptr == item->next_);

  if (nullptr == first_) {
    _ASSERT(nullptr == last_);
    _ASSERT(0 == count_);
    last_ = item;
    first_ = item;
  } else {
    if (index >= 0) {
      Element *p = first_;
      while ((nullptr != p) && (index > 0)) {
        p = p->next_;
        index--;
      }
      if (nullptr != p) {  // insert before p
        item->previous_ = p->previous_;
        item->next_ = p;
        if (nullptr != p->previous_) p->previous_->next_ = item;
        else first_ = item;
        p->previous_ = item;
      } else {  // push back
        last_->next_ = item;
        item->previous_ = last_;
        last_ = item;
      }
    } else {
      Element *p = last_;
      while ((nullptr != p) && (index < -1)) {
        p = p->previous_;
        index++;
      }
      if (nullptr != p) {  // insert after p
        item->next_ = p->next_;
        item->previous_ = p;
        if (nullptr != p->next_) p->next_->previous_ = item;
        else last_ = item;
        p->next_ = item;
      } else {  // push front
        first_->previous_ = item;
        item->next_ = first_;
        first_ = item;
      }
    }
  }

  item->deque_ = this;
  count_++;
}

CountedDeque::Element *CountedDeque::Remove(Element *item) {
  if (item->deque_ != this) return nullptr;

  _ASSERT(count_ > 0);

  if (nullptr != item->previous_) {
    item->previous_->next_ = item->next_;
  } else {
    _ASSERT(first_ == item);
    first_ = item->next_;
  }

  if (nullptr != item->next_) {
    item->next_->previous_ = item->previous_;
  } else {
    _ASSERT(last_ == item);
    last_ = item->previous_;
  }

  count_--;
  _ASSERT(count_ >= 0);

  item->previous_ = nullptr;
  item->next_ = nullptr;
  item->deque_ = nullptr;

  return item;
}

void CountedDeque::Clear() {
  Element *ptr = first_;
  Element *next = nullptr;

  while (nullptr != ptr) {
    next = ptr->next_;
    ptr->previous_ = nullptr;
    ptr->next_ = nullptr;
    ptr->deque_ = nullptr;
    delete ptr;
    ptr = next;
  }

  count_ = 0;
  first_ = nullptr;
  last_ = nullptr;
}

CountedDeque::Element *CountedDeque::operator[](int index) const {
  if (index < 0) index = count_ + index;

  Element *item = nullptr;
  int mid = (count_ + 1) / 2;

  if (index >= 0 && index < mid) {
    item = first_;
    while (index > 0) {
      item = item->next_;
      index--;
    }
  } else if (index >= mid && index < count_) {
    item = last_;
    int max = count_ - 1;
    while (index < max) {
      item = item->previous_;
      index++;
    }
  }

  return item;
}

} // namespace base
} // namespace wiz-vision
