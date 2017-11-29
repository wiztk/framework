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

#ifndef WIZTK_BASE_SIZE_HPP_
#define WIZTK_BASE_SIZE_HPP_

namespace wiztk {
namespace base {

/**
 * @ingroup base
 * @brief Template structure for size in 2 dimension
 */
template<typename T>
struct SizeT {

  inline SizeT()
      : width(T(0)), height(T(0)) {}

  inline SizeT(T width, T height)
      : width(width), height(height) {}

  inline SizeT(const SizeT &other)
      : width(other.width), height(other.height) {}

  inline SizeT &operator=(const SizeT &other) {
    width = other.width;
    height = other.height;
    return *this;
  }

  inline ~SizeT() = default;

  inline bool IsValid() const {
    return width >= 0 && height >= 0;
  }

  inline bool Equal(const SizeT &other) const {
    return (width == other.width) && (height == other.height);
  }

  inline bool Equal(T width, T height) const {
    return (this->width == width && this->height == height);
  }

  inline void Add(int dw, int dh) const {
    width += dw;
    height += dh;
  }

  inline void Subtract(int dw, int dh) const {
    width -= dw;
    height -= dh;
  }

  union { T w, width; };
  union { T h, height; };
};

/**
 * @ingroup base
 */
template<typename T>
inline bool operator==(const SizeT<T> &size1, const SizeT<T> &size2) {
  return ((size1.width == size2.width)
      && (size1.height == size2.height));
}

/**
 * @ingroup base
 */
template<typename T>
inline bool operator!=(const SizeT<T> &size1, const SizeT<T> &size2) {
  return ((size1.width != size2.width) ||
      (size1.height != size2.height));
}

/**
 * @ingroup base
 */
template<typename T>
inline SizeT<T> operator+(const SizeT<T> &size1, const SizeT<T> &size2) {
  return SizeT<T>(size1.width + size2.width, size1.height + size2.height);
}

/**
 * @ingroup base
 */
template<typename T>
inline SizeT<T> operator-(const SizeT<T> &size1, const SizeT<T> &size2) {
  return SizeT<T>(size1.width - size2.width, size1.height - size2.height);
}

template<typename T>
inline SizeT<T> operator*(const SizeT<T> &src, int factor) {
  return SizeT<T>(src.width * factor, src.height * factor);
}

/**
 * @ingroup base
 */
typedef SizeT<int> SizeI;

/**
 * @ingroup base
 */
typedef SizeT<float> SizeF;

/**
 * @ingroup base
 */
typedef SizeT<double> SizeD;

} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_SIZE_HPP_
