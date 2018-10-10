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

#include "wiztk/system/threading/mutex.hpp"

#include "wiztk/system/time/clock.hpp"

#include <stdexcept>

namespace wiztk {
namespace system {
namespace threading {

void Mutex::Lock() {
  if (0 != pthread_mutex_lock(&pthread_mutex_)) {
    throw std::runtime_error("Error! Fail to lock mutex!");
  }
}

void Mutex::Lock(const time::Clock &timeout) {
  if (0 != pthread_mutex_timedlock(&pthread_mutex_, timeout.data())) {
    throw std::runtime_error("Error! Fail to lock mutex with timeout!");
  }
}

void Mutex::TryLock() {
  if (0 != pthread_mutex_trylock(&pthread_mutex_)) {
    throw std::runtime_error("Error! Fail to try lock mutex!");
  }
}

void Mutex::Unlock() {
  if (0 != pthread_mutex_unlock(&pthread_mutex_)) {
    throw std::runtime_error("Error! Fail to unlock mutex!");
  }
}

} // namespace threading
} // namespace system
} // namespace wiztk