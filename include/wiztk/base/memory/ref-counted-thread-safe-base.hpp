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

#ifndef WIZTK_BASE_MEMORY_REF_COUNTED_THREAD_SAFE_BASE_HPP_
#define WIZTK_BASE_MEMORY_REF_COUNTED_THREAD_SAFE_BASE_HPP_

#include "wiztk/base/memory/abstract-ref-counted.hpp"
#include "wiztk/base/memory/atomic-ref-count.hpp"

namespace wiztk {
namespace base {

template<typename TDeleter = AbstractRefCounted::DefaultDeleter>
class RefCountedThreadSafeBase : public AbstractRefCounted {

 public:

  RefCountedThreadSafeBase() = default;

  ~RefCountedThreadSafeBase() override = default;

 protected:

  void Reference() final {
    ++ref_count_;
  }

  void Unreference() final {
    --ref_count_;
  }

  void Destroy() final {
    deleter_(this);
  }

 private:

  AtomicRefCount ref_count_ = {0};
  TDeleter deleter_;

};

} // namespace base
} // namespace wiztk

#endif //WIZTK_REF_COUNTED_THREAD_SAFE_BASE_HPP
