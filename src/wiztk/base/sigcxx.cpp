/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Freeman Zhang <zhanggyb@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "wiztk/base/sigcxx.hpp"

namespace wiztk {
namespace base {

namespace internal {

Binding::~Binding() {
  if (previous) previous->next = next;
  if (next) next->previous = previous;

  if (trackable) {
    if (nullptr == previous) trackable->first_binding_ = next;
    if (nullptr == next) trackable->last_binding_ = previous;
  }

  if (token) {
#ifdef __DEBUG__
    assert(token->binding == this);
#endif
    token->binding = nullptr;
    delete token;
  }
}

Token::~Token() {
  if (trackable) trackable->AuditDestroyingToken(this);

  _ASSERT(nullptr == slot_mark_head.previous());
  Slot::Mark *mark = nullptr;
  while (nullptr != slot_mark_head.next()) {
    mark = static_cast<Slot::Mark *>(slot_mark_head.next());
    mark->slot()->token_ = next;
    mark->slot()->skip_ = true;
    mark->Unlink();
  }

  if (previous) previous->next = next;
  if (next) next->previous = previous;

  if (binding) {
#ifdef __DEBUG__
    assert(binding->token == this);
#endif
    binding->token = nullptr;
    delete binding;
  }
}

}  // namespace details

Trackable::~Trackable() {
  UnbindAllSignals();
}

void Trackable::UnbindSignal(SLOT slot) {
  using internal::Token;

  if (slot->token_->binding->trackable == this) {
    Token *tmp = slot->token_;
//    slot->token_ = slot->token_->next;
//    slot->skip_ = true;

//    slot->running_node_.Unlink();
//    tmp->slot = nullptr;
    delete tmp;
  }
}

void Trackable::UnbindAllSignals() {
  using internal::Binding;

  Binding *tmp = nullptr;
  Binding *it = last_binding_;

  while (it) {
    tmp = it;
    it = it->previous;
    delete tmp;
  }
}

int Trackable::CountSignalBindings() const {
  using internal::Binding;

  int count = 0;
  for (Binding *it = first_binding_; it; it = it->next) {
    count++;
  }
  return count;
}

void Trackable::PushBackBinding(internal::Binding *node) {
#ifdef __DEBUG__
  assert(nullptr == node->trackable);
#endif

  if (last_binding_) {
    last_binding_->next = node;
    node->previous = last_binding_;
  } else {
#ifdef __DEBUG__
    assert(nullptr == first_binding_);
#endif
    node->previous = nullptr;
    first_binding_ = node;
  }
  last_binding_ = node;
  node->next = nullptr;
  node->trackable = this;
}

void Trackable::PushFrontBinding(internal::Binding *node) {
#ifdef __DEBUG__
  assert(nullptr == node->trackable);
#endif

  if (first_binding_) {
    first_binding_->previous = node;
    node->next = first_binding_;
  } else {
#ifdef __DEBUG__
    assert(nullptr == last_binding_);
#endif
    node->next = nullptr;
    last_binding_ = node;
  }
  first_binding_ = node;
  node->previous = nullptr;
  node->trackable = this;
}

void Trackable::InsertBinding(int index, internal::Binding *node) {
#ifdef __DEBUG__
  assert(nullptr == node->trackable);
#endif

  if (nullptr == first_binding_) {
#ifdef __DEBUG__
    assert(nullptr == last_binding_);
#endif
    node->next = nullptr;
    last_binding_ = node;
    first_binding_ = node;
    node->previous = nullptr;
  } else {
    if (index >= 0) {

      internal::Binding *p = first_binding_;
#ifdef __DEBUG__
      assert(p != nullptr);
#endif

      while (p && (index > 0)) {
        p = p->next;
        index--;
      }

      if (p) {  // insert before p

        node->previous = p->previous;
        node->next = p;

        if (p->previous) p->previous->next = node;
        else first_binding_ = node;

        p->previous = node;

      } else {  // push back

        last_binding_->next = node;
        node->previous = last_binding_;
        last_binding_ = node;
        node->next = nullptr;

      }

    } else {

      internal::Binding *p = last_binding_;
#ifdef __DEBUG__
      assert(p);
#endif

      while (p && (index < -1)) {
        p = p->previous;
        index++;
      }

      if (p) {  // insert after p

        node->next = p->next;
        node->previous = p;

        if (p->next) p->next->previous = node;
        else last_binding_ = node;

        p->next = node;

      } else {  // push front

        first_binding_->previous = node;
        node->next = first_binding_;
        first_binding_ = node;
        node->previous = nullptr;

      }

    }
  }
  node->trackable = this;
}

} // namespace base
} // namespace wiztk
