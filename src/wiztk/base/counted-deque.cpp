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

CountedDequeNodeBase::~CountedDequeNodeBase() {
  if (nullptr != deque_) {
    _ASSERT(deque_->count_ > 0);
    deque_->count_--;
    deque_ = nullptr;
  }
}

void CountedDequeNodeBase::OnUnlinked() {
  if (nullptr != deque_) {
    _ASSERT(deque_->count_ > 0);
    deque_->count_--;
    deque_ = nullptr;
  }
}

void CountedDequeNodeBase::Unlink(CountedDequeNodeBase *node) {
  if (nullptr != node->deque_) {
    _ASSERT(node->deque_->count_ > 0);
    node->deque_->count_--;
    node->deque_ = nullptr;
  }

  BinodeBase::Unlink(node);
}

bool CountedDequeNodeBase::IsLinked(CountedDequeNodeBase *node) {
  bool ret = BinodeBase::IsLinked(node);

  if (ret) {
    _ASSERT(nullptr != node->deque_);
  }

  return ret;
}

CountedDequeBase::~CountedDequeBase() {
  _ASSERT(count_ >= 0);
}

} // namespace base
} // namespace wiz-vision
