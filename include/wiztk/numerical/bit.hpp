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

#ifndef WIZTK_NUMERICAL_BIT_HPP_
#define WIZTK_NUMERICAL_BIT_HPP_

namespace wiztk {
namespace numerical {

/**
 * @ingroup numerical
 * @brief Helper class for bit operation
 */
class Bit {

 public:

  template<typename T>
  static inline void Set(T &x, T y) {
    x |= y;
  }

  template<typename T>
  static inline void Clear(T &x, T y) {
    x &= ~y;
  }

  template<typename T>
  static inline void Inverse(T &x, T y) {
    x = (x & (~y)) | (x ^ y);
  }

};

} // namespace numerical
} // namespace wiztk

#endif // WIZTK_NUMERICAL_BIT_HPP_
