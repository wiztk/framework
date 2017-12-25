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

#include "wiztk/gui/input-manager.hpp"

#include "internal/input_private.hpp"

namespace wiztk {
namespace gui {

InputManager::~InputManager() {
  Clear();
}

void InputManager::AddInput(Input *input) {
  auto *node = input->p_.get();
  deque_.push_back(node);
}

void InputManager::Clear() {
  deque_.clear([](base::BinodeBase *obj) {
    auto *node = static_cast<Input::Private *>(obj);
    Input *input = node->proprietor;
    delete input;
  });
}

}
}