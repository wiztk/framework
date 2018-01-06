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

#ifndef WIZTK_SYSTEM_THREADING_MUTEX_HPP_
#define WIZTK_SYSTEM_THREADING_MUTEX_HPP_

#include <pthread.h>

namespace wiztk {
namespace system {
namespace threading {

class Mutex {

 public:

  class Attribute;

 public:

  Mutex() {
    pthread_mutex_init(&pthread_mutex_, nullptr);
  }

  ~Mutex() {
    pthread_mutex_destroy(&pthread_mutex_);
  }

 private:

  pthread_mutex_t pthread_mutex_;

};

class Mutex::Attribute {

 public:

  Attribute() {
    pthread_mutexattr_init(&pthread_mutexattr_);
  }

  ~Attribute() {
    pthread_mutexattr_destroy(&pthread_mutexattr_);
  }

 private:

  pthread_mutexattr_t pthread_mutexattr_;

};

}
}
}

#endif // WIZTK_SYSTEM_THREADING_MUTEX_HPP_
