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

#ifndef WIZTK_BASE_MEMORY_REF_COUNT_HPP_
#define WIZTK_BASE_MEMORY_REF_COUNT_HPP_

#include <cstddef>

namespace wiztk {
namespace base {

/**
 * @ingroup base_memory
 * @brief Reference counter with use_count and weak_count.
 */
class RefCount {

  template<typename T> friend
  class SharedPtr;

  template<typename T> friend
  class WeakPtr;

 public:

  RefCount() {
#ifdef __TEST__
    std::cout << __func__ << std::endl;
#endif
  }

  ~RefCount() {
#ifdef __TEST__
    std::cout << __func__ << std::endl;
#endif
  }

  size_t Increase() {
    return ++use_count_;
  }

  size_t Decrease() {
    return --use_count_;
  }

  size_t use_count() const { return use_count_; }

  size_t weak_count() const { return weak_count_; }

 private:

  size_t use_count_ = 0;

  size_t weak_count_ = 0;

};

}
}

#endif //WIZTK_REF_COUNT_HPP
