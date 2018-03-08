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

#ifndef WIZTK_BASE_MEMORY_SHARED_PTR_HPP_
#define WIZTK_BASE_MEMORY_SHARED_PTR_HPP_

#include "wiztk/base/macros.hpp"

#include "wiztk/base/memory/ref-count.hpp"
#include "wiztk/base/memory/ref-counted-base.hpp"

namespace wiztk {
namespace base {
namespace memory {

/**
 * @ingroup base_memory
 * @brief Shared pointer
 * @tparam T
 */
template<typename T>
class SharedPtr {

  template<typename R> friend
  class WeakPtr;

 public:

  /**
   * @brief Default constructor.
   */
  constexpr SharedPtr() noexcept = default;

  explicit SharedPtr(T *obj) noexcept {
    Reset(obj);
  }

  SharedPtr(const SharedPtr &other) {
    Reset(other.ptr_);
  }

  SharedPtr(SharedPtr &&other) noexcept
      : ptr_(other.ptr_), ref_count_(other.ref_count_) {
    other.ptr_ = nullptr;
    other.ref_count_ = nullptr;
  }

  ~SharedPtr() {
    Reset();
  }

  SharedPtr &operator=(const SharedPtr &other) noexcept {
    Reset(other.ptr_);
    return *this;
  }

  SharedPtr &operator=(SharedPtr &&other) noexcept {
    ptr_ = other.ptr_;
    ref_count_ = other.ref_count_;
    other.ptr_ = nullptr;
    other.ref_count_ = nullptr;
    return *this;
  }

  SharedPtr &operator=(T *obj) noexcept {
    Reset(obj);
    return *this;
  }

  void Reset(T *obj = nullptr) noexcept {
    T *old_ptr = ptr_;
    RefCount *old_ref_count = ref_count_;

    ptr_ = obj;
    ref_count_ = nullptr;

    if (nullptr != ptr_) {
      if (nullptr == ptr_->ref_count_) {
        ptr_->ref_count_ = new RefCount();
        _ASSERT(0 == ptr_->ref_count_->use_count_ && 0 == ptr_->ref_count_->weak_count_);
      }
      ref_count_ = ptr_->ref_count_;
      ptr_->Reference();
    }

    if (nullptr != old_ptr) {
      _ASSERT(old_ptr->ref_count_ == old_ref_count);
      old_ptr->Unreference();
      if (0 == old_ref_count->use_count() && 0 == old_ref_count->weak_count()) {
        delete old_ref_count;
      }
    }
  }

  void Swap(SharedPtr &other) {
    T *old_ptr = ptr_;
    RefCount *old_ref_count = ref_count_;

    ptr_ = other.ptr_;
    ref_count_ = other.ref_count_;

    other.ptr_ = old_ptr;
    other.ref_count_ = old_ref_count;
  }

  T *Get() const noexcept { return ptr_; }

  T &operator*() const noexcept { return *ptr_; }

  T *operator->() const noexcept { return ptr_; }

  size_t use_count() const noexcept {
    return nullptr == ref_count_ ? 0 : ref_count_->use_count();
  }

  size_t weak_count() const noexcept {
    return nullptr == ref_count_ ? 0 : ref_count_->weak_count();
  }

  bool IsUnique() const noexcept {
    return 1 == use_count();
  }

  explicit operator bool() const noexcept {
    if (nullptr == ptr_) {
      _ASSERT(nullptr == ref_count_);
      return false;
    }

    _ASSERT(ptr_->ref_count_ == ref_count_);
    return true;
  }

 private:

  T *ptr_ = nullptr;

  RefCount *ref_count_ = nullptr;

};

/**
 * @ingroup base_memory
 * @brief Constructs an object of tyep T and wraps it in a SharedPtr
 * @tparam T
 * @tparam Args
 * @param args
 * @return
 */
template<typename T, typename ... Args>
inline SharedPtr<T> MakeShared(Args &&... args) {
  return SharedPtr<T>(new T(std::forward<Args>(args)...));
};

} // namespace memory
} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_MEMORY_SHARED_PTR_HPP_
