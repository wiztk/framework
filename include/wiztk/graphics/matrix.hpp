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

#ifndef WIZTK_GRAPHIC_MATRIX_HPP_
#define WIZTK_GRAPHIC_MATRIX_HPP_

#include <memory>

class SkMatrix;

namespace wiztk {
namespace graphics {

/**
 * @ingroup graphics
 * @brief Skia matrix wrapper
 */
class Matrix {

  friend class Canvas;

 public:

  Matrix();

  Matrix(const Matrix &other);

  ~Matrix();

  Matrix &operator=(const Matrix &other);

  const SkMatrix *GetSkMatrix() const;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHIC_MATRIX_HPP_
