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

#include "wiztk/system/threading/main-thread.hpp"

#include "thread_private.hpp"

namespace wiztk {
namespace system {
namespace threading {

MainThread::MainThread()
    : Thread() {
  if (p_->id != Private::kMainThreadID)
    throw std::runtime_error("Error! Not in main thread!");

  if (nullptr != Private::kPerThreadStorage.Get())
    throw std::runtime_error("Error! This object is only allowed to be created once!");

  Private::kPerThreadStorage.Set(this);
  // TODO: Set event loop.
}

MainThread::~MainThread() {
  Private::kPerThreadStorage.Set(nullptr);
}

void MainThread::Run() {
  // The main thread does not use this method.
}

} // namespace threading
} // namespace system
} // namespace wiztk
