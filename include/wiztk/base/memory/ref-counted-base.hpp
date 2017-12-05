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

#ifndef WIZTK_BASE_MEMORY_REF_COUNTED_BASE_HPP_
#define WIZTK_BASE_MEMORY_REF_COUNTED_BASE_HPP_

#include "wiztk/base/memory/abstract-ref-counted.hpp"
#include "wiztk/base/memory/ref-count.hpp"

namespace wiztk {
namespace base {

/**
 * @ingroup base_memory
 * @brief Reference counted base.
 * @tparam TDeleter
 */
template<typename TDeleter = AbstractRefCounted::DefaultDeleter>
class RefCountedBase : public AbstractRefCounted {

  template<typename T> friend
  class SharedPtr;

  template<typename T> friend
  class WeakPtr;

 public:

  RefCountedBase() = default;

  ~RefCountedBase() override = default;

 protected:

  void Reference() final {
    if (nullptr == ref_count_) return;

    ref_count_->Increase();
  }

  void Unreference() final {
    if (nullptr == ref_count_) return;

    if (0 == ref_count_->Decrease()) {
      Destroy();
    }
  }

  void Destroy() final {
    deleter_(this);
  }

 private:

  /*
   * A pointer to a RefCount object, this is only created in SharedPtr and destroyed in
   * SharedPtr or WeakPtr when use_count and weak_count are both 0.
   */
  RefCount *ref_count_ = nullptr;

  TDeleter deleter_;

};

} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_MEMORY_REF_COUNTED_BASE_HPP_
