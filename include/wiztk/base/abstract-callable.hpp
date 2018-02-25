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

#ifndef WIZTK_BASE_ABSTRACT_CALLABLE_HPP_
#define WIZTK_BASE_ABSTRACT_CALLABLE_HPP_

#include "wiztk/base/macros.hpp"

namespace wiztk {
namespace base {

template<typename SIGNATURE>
class AbstractCallable;

/**
 * @ingroup base
 * @brief Abstract callable.
 * @tparam ReturnType
 * @tparam ParamTypes
 */
template<typename ReturnType, typename ... ParamTypes>
class AbstractCallable<ReturnType(ParamTypes...)> {

 public:

  AbstractCallable() = default;

  virtual ~AbstractCallable() = default;

  virtual ReturnType Call(ParamTypes... Args) = 0;

};

} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_ABSTRACT_CALLABLE_HPP_
