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

#include <wiztk/gui/anchor.hpp>
#include <wiztk/gui/anchor-group.hpp>
#include <wiztk/base/macros.hpp>

#include "internal/abstract-view_iterators.hpp"

namespace wiztk {
namespace gui {

Anchor::Anchor(AbstractView *view)
    : previous_(nullptr),
      next_(nullptr),
      group_(nullptr),
      contrary_(nullptr) {}

Anchor::~Anchor() {
  if (group_) group_->Remove(this);

  _ASSERT(nullptr == group_);
  _ASSERT(nullptr == previous_);
  _ASSERT(nullptr == next_);

  if (contrary_) {
    contrary_->contrary_ = nullptr;
    delete contrary_;
  }
}

std::pair<Anchor *, Anchor *> Anchor::MakePair(int distance,
                                               AbstractView *view1,
                                               AbstractView *view2) {
  AbstractView::Iterator it1(view1);
  AbstractView::Iterator it2(view2);

  bool valid = it1.parent() == view2 || view1 == it2.parent() || it1.parent() == it2.parent();

  if (!valid)
    throw std::runtime_error("Error! Cannot make anchor pair between 2 views without any relationship!");

  Anchor *anchor1 = new Anchor(view1);
  Anchor *anchor2 = new Anchor(view2);
  anchor1->contrary_ = anchor2;
  anchor2->contrary_ = anchor1;
  anchor1->distance_.reset(new int);
  anchor2->distance_ = anchor1->distance_;
  *anchor1->distance_ = distance;

  return std::make_pair(anchor1, anchor2);
}

} // namespace gui
} // namespace wiztk
