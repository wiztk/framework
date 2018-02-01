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
namespace memory {

/**
 * @ingroup base_memory
 * @brief Atomic ref count used for thread safe.
 */
class WIZTK_EXPORT AtomicRefCount {

 public:

  /**
   * @brief Declare this class is non-copyable and non-movable.
   */
  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(AtomicRefCount);

  /**
   * @brief Default constructor.
   */
  constexpr AtomicRefCount()
      : ref_count_(0) {}

  /**
   * @brief Constructor with an initial count.
   * @param value
   */
  explicit constexpr AtomicRefCount(size_t value)
      : ref_count_(value) {}

  /**
   * @brief Default destructor.
   */
  ~AtomicRefCount() = default;

  /**
   * @brief Increase a reference count and return the new value.
   */
  size_t Reference() {
    return Reference(1);
  }

  /**
   * @brief Increase the reference count by given value, which must exceed 0.
   * @param value
   */
  size_t Reference(size_t value) {
    return ref_count_.fetch_add(value, std::memory_order_relaxed);
  }

  /**
   * @brief Decrease the reference count and return the new value.
   * @return
   */
  size_t Unreference() {
    return ref_count_.fetch_sub(1, std::memory_order_acq_rel);
  }

  /**
   * @brief Increase the reference count.
   * @return
   */
  AtomicRefCount &operator++() {
    Reference();
    return *this;
  }

  /**
   * @brief Decrease the reference count.
   * @return
   */
  AtomicRefCount &operator--() {
    Unreference();
    return *this;
  }

  /**
   * @brief Return whether the reference count is one.
   * @return
   */
  bool IsOne() const {
    return 1 == ref_count_.load(std::memory_order_acquire);
  }

  /**
   * @brief Return wheter the reference count is zero.
   * @return
   */
  bool IsZero() const {
    return 0 == ref_count_.load(std::memory_order_acquire);
  }

  /**
   * @brief Get the reference count.
   * @return
   */
  size_t ref_count() const {
    return ref_count_.load(std::memory_order_acquire);
  }

 private:

  typedef std::atomic_ulong atomic_size_t;

  atomic_size_t ref_count_;

};

} // namespace memory
} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_MEMORY_ATOMIC_REF_COUNT_HPP_
