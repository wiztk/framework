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

#include "wiztk/system/threading/thread-attribute.hpp"

namespace wiztk {
namespace system {
namespace threading {

ThreadAttribute::ThreadAttribute() {
  pthread_attr_init(&pthread_attribute_);
}

ThreadAttribute::~ThreadAttribute() {
  pthread_attr_destroy(&pthread_attribute_);
}

void ThreadAttribute::SetDetachState(DetachStateType state_type) {
  pthread_attr_setdetachstate(&pthread_attribute_, state_type);
}

ThreadAttribute::DetachStateType ThreadAttribute::GetDetachState() const {
  int val = 0;
  pthread_attr_getdetachstate(&pthread_attribute_, &val);
  return static_cast<DetachStateType>(val);
}

void ThreadAttribute::SetScope(ScopeType scope_type) {
  pthread_attr_setscope(&pthread_attribute_, scope_type);
}

ThreadAttribute::ScopeType ThreadAttribute::GetScope() const {
  int val = 0;
  pthread_attr_getscope(&pthread_attribute_, &val);
  return static_cast<ScopeType>(val);
}

} // namespace threading
} // namespace system
} // namespace wiztk
