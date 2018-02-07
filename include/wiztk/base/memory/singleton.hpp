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

#ifndef WIZTK_BASE_MEMORY_SINGLETON_HPP_
#define WIZTK_BASE_MEMORY_SINGLETON_HPP_

#include "wiztk/base/macros.hpp"

namespace wiztk {
namespace base {
namespace memory {

/**
 * @ingroup base_memory
 * @brief Default traits for Singleton.
 * @tparam T
 */
template<typename T>
struct DefaultSingletonTraits {

  static T *New() {
    return new T();
  }

  static void Delete(T *obj) {
    delete obj;
  }

};

namespace internal {

/**
 * @ingroup base_memory_intern
 * @brief Template class manages a single instance of given type.
 * @tparam T
 * @tparam Traits
 *
 * @todo Make the methods in this class thread safe.
 */
template<typename T, typename Traits = DefaultSingletonTraits<T> >
class SingletonBase {

  template<typename AliasT, typename AliasTraits> friend
  class Singleton;

  static T *CreateOnce() {
    if (nullptr == kInstance)
      kInstance = Traits::New();

    return kInstance;
  }

  static void Release() {
    Traits::Delete(kInstance);
    kInstance = nullptr;
  }

  static void OnExit(void *) {
    Traits::Delete(kInstance);
    kInstance = nullptr;
  }

  static T *kInstance;

};

template<typename T, typename Traits>
T *SingletonBase<T, Traits>::kInstance = nullptr;

} // namespace internal

/**
 * @ingroup base_memory
 * @brief Template class manages a single instance of given type.
 * @tparam T
 * @tparam Traits
 */
template<typename T, typename Traits = DefaultSingletonTraits<T> >
class Singleton {

  friend T *T::GetInstance();
  friend T *T::GetOrCreateInstance();
  friend void T::ReleaseInstance();

  /**
   * @brief Directly get the instance pointer.
   * @return
   */
  static inline T *get() {
    return internal::SingletonBase<T, Traits>::kInstance;
  }

  /**
   * @brief Create an instance once.
   * @return
   */
  static inline T *get_or_create() {
    return internal::SingletonBase<T, Traits>::CreateOnce();
  }

  /**
   * @brief Release the instance and reset the static pointer.
   */
  static inline void release() {
    internal::SingletonBase<T, Traits>::Release();
  }

};

} // namespace memory
} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_MEMORY_SINGLETON_HPP_
