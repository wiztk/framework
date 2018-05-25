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

#include "matrix/private.hpp"

namespace wiztk {
namespace graphics {

Matrix::Matrix() {
  p_.reset(new Private);
}

Matrix::Matrix(const Matrix &other) {
  p_.reset(new Private(*other.p_));
}

Matrix::~Matrix() {
}

Matrix &Matrix::operator=(const Matrix &other) {
  *p_ = *other.p_;
  return *this;
}

const SkMatrix *Matrix::GetSkMatrix() const {
  return &p_->sk_matrix;
}

} // namespace graphics
} // namespace wiztk
