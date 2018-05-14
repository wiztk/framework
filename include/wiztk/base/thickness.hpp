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

#ifndef WIZTK_BASE_THICKNESS_HPP_
#define WIZTK_BASE_THICKNESS_HPP_

namespace wiztk {
namespace base {

/**
 * @ingroup base
 * @brief Structure defining thickness around the edges of a rectangle.
 */
template<typename T>
struct Thickness {

  inline Thickness()
      : l(T(0)), t(T(0)), r(T(0)), b(T(0)) {}

  inline explicit Thickness(T value)
      : l(value), t(value), r(value), b(value) {}

  inline Thickness(T l, T t, T r, T b)
      : l(l), t(t), r(r), b(b) {}

  inline Thickness(const Thickness &other)
      : l(other.l), t(other.t), r(other.r), b(other.b) {}

  inline ~Thickness() = default;

  inline Thickness &operator=(const Thickness &other) {
    l = other.l;
    t = other.t;
    r = other.r;
    b = other.b;
    return *this;
  }

  inline bool Equal(T l, T t, T r, T b) {
    return this->l == l
        && this->t == t
        && this->r == r
        && this->b == b;
  }

  /**
   * @brief The sum of left and right.
   */
  inline T horizontal() const {
    return left + right;
  }

  /**
   * @brief The sum of top and bottom.
   */
  inline T vertical() const {
    return top + bottom;
  }

  union { T l, left; };
  union { T t, top; };
  union { T r, right; };
  union { T b, bottom; };

};

/**
 * @ingroup base
 */
template<typename T>
inline bool operator==(const Thickness<T> &s1, const Thickness<T> &s2) {
  return memcmp(&s1, &s2, sizeof(Thickness<T>)) == 0;
}

/**
 * @ingroup base
 */
template<typename T>
inline bool operator!=(const Thickness<T> &s1, const Thickness<T> &s2) {
  return memcmp(&s1, &s2, sizeof(Thickness<T>)) != 0;
}

typedef Thickness<int> ThicknessI;
typedef Thickness<float> ThicknessF;
typedef Thickness<double> ThicknessD;

} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_THICKNESS_HPP_
