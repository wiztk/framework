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

#ifndef WIZTK_BASE_ABSTRACT_RUNNABLE_HPP_
#define WIZTK_BASE_ABSTRACT_RUNNABLE_HPP_

#include "wiztk/base/macros.hpp"

namespace wiztk {
namespace base {

/**
 * @ingroup base
 * @brief Virtual base class of runnable objects.
 * @tparam ParamTypes
 */
template<typename ... ParamTypes>
class WIZTK_EXPORT AbstractRunnable {

 public:

  /**
   * @brief Default constructor.
   */
  AbstractRunnable() = default;

  /**
   * @brief Default copy constructor.
   */
  AbstractRunnable(const AbstractRunnable &) = default;

  /**
   * @brief Default move constructor.
   */
  AbstractRunnable(AbstractRunnable &&) noexcept = default;

  /**
   * @brief Default destructor.
   */
  virtual ~AbstractRunnable() = default;

  /**
   * @brief Default copy assignment.
   * @return Reference to this object.
   */
  AbstractRunnable &operator=(const AbstractRunnable &) = default;

  /**
   * @brief Default move assignment.
   * @return Reference to this object.
   */
  AbstractRunnable &operator=(AbstractRunnable &&) noexcept = default;

  /**
   * @brief The virtual method to be overrided in subclass.
   * @param Args Variadic template arguments.
   */
  virtual void Run(ParamTypes... Args) = 0;

};

} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_ABSTRACT_RUNNABLE_HPP_
