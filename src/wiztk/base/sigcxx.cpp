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

#include "wiztk/base/sigcxx.hpp"

namespace wiztk {
namespace base {

namespace internal {

BindingNode::~BindingNode() {
  if (nullptr != token) {
    _ASSERT(token->binding == this);
    token->binding = nullptr;
    delete token;
  }
}

TokenNode::~TokenNode() {
  _ASSERT(nullptr == slot_mark_head.previous());
  Slot::Mark *mark = nullptr;
  while (nullptr != slot_mark_head.next()) {
    mark = static_cast<Slot::Mark *>(slot_mark_head.next());  // always a Slot::Mark pointer
    ++mark->slot()->iterator_;
    ++mark->slot()->ref_count_;
    mark->unlink();
  }

  if (nullptr != binding) {
    _ASSERT(binding->token == this);
    binding->token = nullptr;
    delete binding;
  }
}

}  // namespace internal

Trackable::Trackable(const Trackable &)
    : Trackable() {}

Trackable::~Trackable() {
  UnbindAllSignals();
}

void Trackable::UnbindSignal(SLOT slot) {
  using internal::TokenNode;

  if (slot->iterator_->binding->trackable == this) {
    TokenNode *tmp = slot->iterator_.get();
    delete tmp;
  }
}

void Trackable::UnbindAllSignals() {
  internal::BindingNode *tmp = nullptr;

  auto it = bindings_.rbegin();
  while (it) {
    tmp = it.get();
    delete tmp;
    it = bindings_.rbegin();
  }
}

size_t Trackable::CountSignalBindings() const {
  size_t count = 0;
  for (auto it = bindings_.cbegin(); it != bindings_.cend(); ++it) {
    count++;
  }
  return count;
}

// ------

} // namespace base
} // namespace wiztk
