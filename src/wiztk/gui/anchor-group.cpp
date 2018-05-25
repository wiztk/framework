/*
 * Copyright 2017 - 2018 The WizTK Authors.
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

#include <wiztk/gui/anchor-group.hpp>
#include <wiztk/gui/anchor.hpp>

namespace wiztk {
namespace gui {

AnchorGroup::AnchorGroup(AbstractView *view, Alignment align)
    : view_(view),
      alignment_(align),
      first_(nullptr),
      last_(nullptr),
      count_(0) {

}

AnchorGroup::~AnchorGroup() {
  Clear();
}

void AnchorGroup::PushFront(Anchor *anchor) {
  _ASSERT(nullptr == anchor->group_);
  _ASSERT(nullptr == anchor->previous_);
  _ASSERT(nullptr == anchor->next_);

  if (first_) {
    first_->previous_ = anchor;
    anchor->next_ = first_;
  } else {
    _ASSERT(nullptr == last_);
    _ASSERT(0 == count_);
  }
  first_ = anchor;

  anchor->group_ = this;
  count_++;
}

void AnchorGroup::PushBack(Anchor *anchor) {
  _ASSERT(nullptr == anchor->group_);
  _ASSERT(nullptr == anchor->previous_);
  _ASSERT(nullptr == anchor->next_);

  if (last_) {
    last_->next_ = anchor;
    anchor->previous_ = last_;
  } else {
    _ASSERT(nullptr == first_);
    _ASSERT(0 == count_);
    first_ = anchor;
  }
  last_ = anchor;

  anchor->group_ = this;
  count_++;
}

void AnchorGroup::Insert(Anchor *anchor, int index) {
  _ASSERT(nullptr == anchor->group_);
  _ASSERT(nullptr == anchor->previous_);
  _ASSERT(nullptr == anchor->next_);

  if (nullptr == first_) {
    _ASSERT(nullptr == last_);
    _ASSERT(0 == count_);
    last_ = anchor;
    first_ = anchor;
  } else {
    if (index >= 0) {
      Anchor *p = first_;
      while (p && (index > 0)) {
        p = p->next_;
        index--;
      }
      if (p) {  // insert before p
        anchor->previous_ = p->previous_;
        anchor->next_ = p;
        if (p->previous_) p->previous_->next_ = anchor;
        else first_ = anchor;
        p->previous_ = anchor;
      } else {  // push back
        last_->next_ = anchor;
        anchor->previous_ = last_;
        last_ = anchor;
      }
    } else {
      Anchor *p = last_;
      while (p && (index < -1)) {
        p = p->previous_;
        index++;
      }
      if (p) {  // insert after p
        anchor->next_ = p->next_;
        anchor->previous_ = p;
        if (p->next_) p->next_->previous_ = anchor;
        else last_ = anchor;
        p->next_ = anchor;
      } else {  // push front
        first_->previous_ = anchor;
        anchor->next_ = first_;
        first_ = anchor;
      }
    }
  }

  anchor->group_ = this;
  count_++;
}

Anchor *AnchorGroup::Remove(Anchor *anchor) {
  if (anchor->group_ != this) return nullptr;

  _ASSERT(count_ > 0);

  if (anchor->previous_) {
    anchor->previous_->next_ = anchor->next_;
  } else {
    _ASSERT(first_ == anchor);
    first_ = anchor->next_;
  }

  if (anchor->next_) {
    anchor->next_->previous_ = anchor->previous_;
  } else {
    _ASSERT(last_ == anchor);
    last_ = anchor->previous_;
  }

  count_--;
  _ASSERT(count_ >= 0);

  anchor->previous_ = nullptr;
  anchor->next_ = nullptr;
  anchor->group_ = nullptr;

  return anchor;
}

void AnchorGroup::Clear() {
  Anchor *ptr = first_;
  Anchor *next = nullptr;

  while (ptr) {
    next = ptr->next_;
    ptr->previous_ = nullptr;
    ptr->next_ = nullptr;
    ptr->group_ = nullptr;
    delete ptr;
    ptr = next;
  }

  count_ = 0;
  first_ = nullptr;
  last_ = nullptr;
}

} // namespace gui
} // namespace wiztk

