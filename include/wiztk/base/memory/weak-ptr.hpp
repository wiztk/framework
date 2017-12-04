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

#ifndef WIZTK_BASE_MEMORY_WEAK_PTR_HPP_
#define WIZTK_BASE_MEMORY_WEAK_PTR_HPP_

#include "wiztk/base/memory/ref-counted-base.hpp"
#include "wiztk/base/memory/shared-ptr.hpp"

namespace wiztk {
namespace base {

template<typename T>
class WeakPtr {

 public:

  constexpr WeakPtr() noexcept = default;

  WeakPtr(const WeakPtr &other) noexcept
      : ptr_(other.ptr_), ref_count_(other.ref_count_) {
    if (nullptr != ref_count_) {
      _ASSERT(nullptr != ptr_ && ptr_->ref_count_ == ref_count_);
      ++ref_count_->weak_count_;
    }
  }

  explicit WeakPtr(const SharedPtr<T> &shared_ptr)
      : ptr_(shared_ptr.ptr_), ref_count_(shared_ptr.ref_count_) {
    if (nullptr != ref_count_) {
      _ASSERT(nullptr != ptr_);
      ++ref_count_->weak_count_;
    }
  }

  ~WeakPtr() {
    Reset();
  }

  WeakPtr &operator=(const SharedPtr<T> &other) noexcept {
//    T *old_ptr = ptr_;
    RefCount *old_ref_count = ref_count_;

    ptr_ = other.ptr_;
    ref_count_ = other.ref_count_;

    if (nullptr != ref_count_) {
      ++ref_count_->weak_count_;
    }

    if (nullptr != old_ref_count) {
      --old_ref_count->weak_count_;

      if ((0 == old_ref_count->use_count()) && (0 == old_ref_count->weak_count())) {
        delete old_ref_count;
      }
    }

    return *this;
  }

  WeakPtr &operator=(const WeakPtr &other) noexcept {
//    T *old_ptr = ptr_;
    RefCount *old_ref_count = ref_count_;

    ptr_ = other.ptr_;
    ref_count_ = other.ref_count_;

    if (nullptr != ref_count_) {
      ++ref_count_->weak_count_;
    }

    if (nullptr != old_ref_count) {
      --old_ref_count->weak_count_;

      if ((0 == old_ref_count->use_count()) && (0 == old_ref_count->weak_count())) {
        delete old_ref_count;
      }
    }

    return *this;
  }

  void Reset() {
    ptr_ = nullptr;

    if (nullptr != ref_count_) {
      --ref_count_->weak_count_;

      if ((0 == ref_count_->use_count()) && (0 == ref_count_->weak_count())) {
        delete ref_count_;
      }

      ref_count_ = nullptr;
    }
  }

  void Swap(WeakPtr &other) {
    T *tmp_ptr = ptr_;
    RefCount *tmp_ref_count = ref_count_;

    ptr_ = other.ptr_;
    ref_count_ = other.ref_count_;

    other.ptr_ = tmp_ptr;
    other.ref_count_ = tmp_ref_count;
  }

  SharedPtr<T> Lock() const noexcept {
    if (ref_count_->use_count() > 0) return SharedPtr<T>(ptr_);

    return SharedPtr<T>();
  }

  size_t use_count() const {
    return nullptr == ref_count_ ? 0 : ref_count_->use_count();
  }

  size_t weak_count() const {
    return nullptr == ref_count_ ? 0 : ref_count_->weak_count();
  }

  explicit operator bool() const noexcept {
    if (nullptr != ref_count_) {
      _ASSERT(0 != ref_count_->use_count() || 0 != ref_count_->weak_count());
      return true;
    }

    return false;
  }

 private:

  T *ptr_ = nullptr;

  RefCount *ref_count_ = nullptr;

};

} // namespace base
} // namespace wiztk

#endif //WIZTK_WEAK_PTR_HPP
