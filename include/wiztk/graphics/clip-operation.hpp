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

#ifndef WIZTK_GRAPHIC_CLIP_OPERATION_HPP_
#define WIZTK_GRAPHIC_CLIP_OPERATION_HPP_

namespace wiztk {
namespace graphics {

/**
 * @ingroup graphic
 * @brief Clip operations
 */
enum ClipOperation {
  kClipDifference,  // 0
  kClipIntersect, // 1
  kClipUnion,
  kClipXOR,

};

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHIC_CLIP_OPERATION_HPP_
