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

#include "wiztk/base/counted-deque.hpp"

#include "wiztk/base/macros.hpp"

namespace wiztk {
namespace base {

namespace internal {

void CountedDequeNodeTraits::OnUnlinked() {
  if (nullptr != node_->deque_) {
    _ASSERT(node_->deque_->count_ > 0);
    --node_->deque_->count_;
    node_->deque_ = nullptr;
  }
}

}

// -----

void CountedDequeNodeBase::PushFront(CountedDequeNodeBase *node, CountedDequeNodeBase *other) {
  if (other == node) return;
  if (node->traits_.previous_ == &other->traits_) return;

  internal::CountedDequeNodeTraits::PushFront(&node->traits_, &other->traits_);
  _ASSERT(other->deque_ == nullptr);

  other->deque_ = node->deque_;
  if (nullptr != node->deque_) ++node->deque_;
}

void CountedDequeNodeBase::PushBack(CountedDequeNodeBase *node, CountedDequeNodeBase *other) {
  if (other == node) return;;
  if (node->traits_.next_ == &other->traits_) return;

  internal::CountedDequeNodeTraits::PushBack(&node->traits_, &other->traits_);
  _ASSERT(other->deque_ == nullptr);

  other->deque_ = node->deque_;
  if (nullptr != node->deque_) ++node->deque_;
}

void CountedDequeNodeBase::Unlink(CountedDequeNodeBase *node) {
  internal::CountedDequeNodeTraits::Unlink(&node->traits_);
  _ASSERT(node->deque_ == nullptr);
}

bool CountedDequeNodeBase::IsLinked(const CountedDequeNodeBase *node) {
  bool ret = internal::CountedDequeNodeTraits::IsLinked(&node->traits_);
  if (ret) _ASSERT(nullptr != node->deque_);
  return ret;
}

CountedDequeBase::~CountedDequeBase() {
  _ASSERT(count_ >= 0);
}

} // namespace base
} // namespace wiz-vision
