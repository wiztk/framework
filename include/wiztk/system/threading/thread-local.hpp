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

#ifndef WIZTK_SYSTEM_THREADING_THREAD_LOCAL_HPP_
#define WIZTK_SYSTEM_THREADING_THREAD_LOCAL_HPP_

#include <pthread.h>

#include <stdexcept>

namespace wiztk {
namespace system {
namespace threading {

/**
 * @ingroup system_threading
 * @brief Per-thread local storage of data.
 * @tparam T Specifies the type of data stored per-thread.
 */
template<typename T>
class ThreadLocal {

 public:

  /**
   * @brief Default constructor.
   */
  ThreadLocal() {
    if (0 != pthread_key_create(&native_, nullptr)) {
      throw std::runtime_error("Error! Cannot create per-thread local object!");
    }
  }

  // TODO: another constructor with deleter

  /**
   * @brief Destructor.
   */
  virtual ~ThreadLocal() {
    pthread_key_delete(native_);
  }

  /**
   * @brief Sets the value of this instance for the current thread.
   * @param data
   */
  void Set(const T *data) {
    if (0 != pthread_setspecific(native_, data)) {
      throw std::runtime_error("Error! Cannot set per-thread local data!");
    }
  }

  /**
   * @brief Gets the value of this instance for the current thread.
   * @return
   */
  T *Get() const {
    return static_cast<T *>(pthread_getspecific(native_));
  }

 private:

  pthread_key_t native_ = 0;

};

} // namespace threading
} // namespace system
} // namespace wiztk

#endif // WIZTK_SYSTEM_THREADING_THREAD_LOCAL_HPP_
