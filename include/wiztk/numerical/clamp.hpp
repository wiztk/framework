/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
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

#ifndef WIZTK_CLAMP_HPP
#define WIZTK_CLAMP_HPP

#include <algorithm>

namespace wiztk {
namespace numerical {

/**
 * @ingroup base
 * @brief Clamp given value between min and max
 */
template<typename T>
inline T Clamp(T x, T min_value, T max_value) {
  return std::min(std::max(x, min_value), max_value);
}

} // namespace numerical
} // namespace wiztk

#endif //WIZTK_CLAMP_HPP
