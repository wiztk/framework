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

#ifndef WIZTK_BASE_POINT_HPP_
#define WIZTK_BASE_POINT_HPP_

#include "wiztk/base/vector.hpp"

namespace wiztk {
namespace base {

/**
 * @ingroup base
 * @brief Point of integer in 2 dimension.
 */
typedef Vector2<int> Point2I;

/**
 * @ingroup base
 * @brief Point of float in 2 dimension.
 */
typedef Vector2<float> Point2F;

/**
 * @ingroup base
 * @brief Point of double in 2 dimension.
 */
typedef Vector2<double> Point2D;

/**
 * @ingroup base
 * @brief Point of integer in 3 dimension.
 */
typedef Vector3<int> Point3I;

/**
 * @ingroup base
 * @brief Point of float in 3 dimension.
 */
typedef Vector3<float> Point3F;

/**
 * @ingroup base
 * @brief Point of double in 3 dimension.
 */
typedef Vector3<double> Point3D;

} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_POINT_HPP_
