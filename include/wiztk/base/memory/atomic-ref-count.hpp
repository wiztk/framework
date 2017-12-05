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

#ifndef WIZTK_BASE_MEMORY_ATOMIC_REF_COUNT_HPP_
#define WIZTK_BASE_MEMORY_ATOMIC_REF_COUNT_HPP_

#include "wiztk/base/macros.hpp"

#include <atomic>

namespace wiztk {
namespace base {

/**
 * @ingroup base_memory
 * @brief Atomic ref count used for thread safe.
 */
class WIZTK_EXPORT AtomicRefCount {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(AtomicRefCount);

  constexpr AtomicRefCount() noexcept
      : ref_count_(0) {}

  explicit constexpr AtomicRefCount(size_t value) noexcept
      : ref_count_(value) {}

  ~AtomicRefCount() = default;

  AtomicRefCount &operator++() {
    ref_count_.fetch_add(1, std::memory_order_relaxed);
    return *this;
  }

  AtomicRefCount &operator--() {
    ref_count_.fetch_sub(1, std::memory_order_acq_rel);
    return *this;
  }

  size_t ref_count() const { return ref_count_.load(std::memory_order_relaxed); }

 private:

  std::atomic_ulong ref_count_;

};

}
}

#endif // WIZTK_BASE_ATOMIC_REF_COUNT_HPP_
