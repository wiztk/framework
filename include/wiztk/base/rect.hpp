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

#ifndef WIZTK_CORE_RECT_HPP_
#define WIZTK_CORE_RECT_HPP_

#include "point.hpp"
#include "size.hpp"

#include <cstring>
#include <algorithm>

namespace wiztk {

namespace base {

/**
 * @ingroup base
 * @brief Template class for rectangle with 4 edges (left, top, right, bottom).
 */
template<typename T>
struct Rect {

  /**
   * @brief Create a new rectangle from the specified values of left/top/right/bottom
   * @param left
   * @param top
   * @param right
   * @param bottom
   * @return
   */
  inline static Rect MakeFromLTRB(T left, T top, T right, T bottom) {
    return Rect(left, top, right, bottom);
  }

  /**
   * @brief Create a new rectangle from the specified values of x/y/width/height
   * @param x
   * @param y
   * @param width
   * @param height
   * @return
   */
  inline static Rect MakeFromXYWH(T x, T y, T width, T height) {
    return Rect(x, y, x + width, y + height);
  }

  /**
   * @brief Default constructor
   *
   * Create an empty rectangle
   */
  inline Rect()
      : left(0), top(0), right(0), bottom(0) {}

  /**
   * @brief Construct a Rect with specified LTRB values
   * @param left
   * @param top
   * @param right
   * @param bottom
   */
  inline Rect(T left, T top, T right, T bottom)
      : left(left), top(top), right(right), bottom(bottom) {}

  /**
   * @brief Construct a Rect with width and height
   * @param width
   * @param height
   */
  inline Rect(T width, T height)
      : left(0), top(0), right(width), bottom(height) {}

  /**
   * @brief Copy constructor
   * @param other
   */
  inline Rect(const Rect &other)
      : left(other.left), top(other.top), right(other.right), bottom(other.bottom) {}

  /**
   * @brief Copy constructor
   * @tparam R
   * @param other
   */
  template<typename R>
  inline Rect(const Rect<R> &other)
      : left(T(other.left)), top(T(other.top)), right(T(other.right)), bottom(T(other.bottom)) {}

  /**
   * @brief Overload the assignment operator
   * @param other
   * @return
   */
  inline Rect &operator=(const Rect &other) {
    left = other.left;
    top = other.top;
    right = other.right;
    bottom = other.bottom;
    return *this;
  }

  /**
   * @brief Overload the assignment operator
   * @tparam R
   * @param other
   * @return
   */
  template<typename R>
  inline Rect &operator=(const Rect<R> &other) {
    left = T(other.left);
    top = T(other.top);
    right = T(other.right);
    bottom = T(other.bottom);
    return *this;
  }

  /**
   * @brief Returns a boolean that indicates whether the rectangle is empty
   * @return
   *    - true Empty, width of height is 0 or negative
   *    - false Not empty
   */
  inline bool IsEmpty() const {
    return (width() <= T(0)) || (height() <= T(0));
  }

  /**
   * @brief Returns a boolean that indicates whether the given point is contained in this rectangle
   * @param x
   * @param y
   * @return
   */
  inline bool Contain(T x, T y) const {
    return left <= x && x < right && top <= y && y < bottom;
  }

  /**
   * @brief Returns a boolean value that indicates whether this rect completely encloses another
   * @param other
   * @return
   */
  inline bool Contain(const Rect &other) const {
    return left <= other.left &&
        top <= other.top &&
        right >= other.right &&
        bottom >= other.bottom;
  }

  /**
   * @brief Move the position of this rectangle to the given position
   * @param x
   * @param y
   */
  inline void MoveTo(T x, T y) {
    right = x + width();
    bottom = y + height();
    left = x;
    top = y;
  }

  /**
   * @brief Relatively move
   * @param dx
   * @param dy
   */
  inline void Move(T dx, T dy) {
    left += dx;
    top += dy;
    right += dx;
    bottom += dy;
  }

  /**
   * @brief Resize this rectangle
   * @param width
   * @param height
   */
  inline void Resize(T width, T height) {
    right = left + width;
    bottom = top + height;
  }

  /**
   * @brief Insets a rectangle by a specified value
   * @param val
   * @return
   */
  inline Rect Inset(T val) const {
    return Inset(val, val);
  }

  /**
   * @brief Insets a rectangle by specified values
   * @param dx
   * @param dy
   * @return
   */
  inline Rect Inset(T dx, T dy) const {
    return Rect(left + dx, top + dy, right - dx, bottom - dy);
  }

  /**
   * @brief Returns a boolean if this rect intersects another
   * @param other
   * @return
   */
  inline bool Intersect(const Rect &other) const {
    return !GetIntersection(*this, other).IsEmpty();
  }

  /**
   * @brief Get the intersection of two rectangles
   * @param rect1
   * @param rect2
   * @return
   */
  static inline Rect GetIntersection(const Rect &rect1, const Rect &rect2) {
    return Rect(std::max(rect1.left, rect2.left),
                std::max(rect1.top, rect2.top),
                std::min(rect1.right, rect2.right),
                std::min(rect1.bottom, rect2.bottom));
  }

  /**
   * @brief Get the left coordinate
   * @return
   */
  inline T x() const { return left; }

  /**
   * @brief Get the top coordinate
   * @return
   */
  inline T y() const { return top; }

  /**
   * @brief Get the width
   * @return
   */
  inline T width() const { return right - left; }

  /**
   * @brief Get the height
   * @return
   */
  inline T height() const { return bottom - top; }

  /**
   * @brief Get the center of x
   * @return
   */
  inline T center_x() const { return left + width() / T(2.0); }

  /**
   * @brief Get the center of y
   * @return
   */
  inline T center_y() const { return top + height() / T(2.0); }

  /**
   * @brief Left
   */
  union { T l, left, x0; };

  /**
   * @brief Top
   */
  union { T t, top, y0; };

  /**
   * @brief Right
   */
  union { T r, right, x1; };

  /**
   * @brief Bottom
   */
  union { T b, bottom, y1; };

};

/**
 * @ingroup base
 * @brief Multiple a given rect with a scalar number
 * @tparam T
 * @param src
 * @param factor
 * @return
 */
template<typename T>
inline Rect<T> operator*(const Rect<T> &src, int factor) {
  return Rect<T>(src.left * factor, src.top * factor, src.right * factor, src.bottom * factor);
}

/**
 * @ingroup base
 * @brief Compare twe rectangle objects
 * @tparam T
 * @param src
 * @param dst
 * @return
 */
template<typename T>
inline bool operator==(const Rect<T> &src, const Rect<T> &dst) {
  return memcmp(&src, &dst, sizeof(Rect<T>)) == 0;
}

template<typename T>
inline bool operator!=(const Rect<T> &src, const Rect<T> &dst) {
  return memcmp(&src, &dst, sizeof(Rect<T>)) != 0;
}

/**
 * @ingroup base
 * @brief A typedef to Rect with integer values
 */
typedef Rect<int> RectI;

/**
 * @ingroup base
 * @brief A typedef to Rect with float values
 */
typedef Rect<float> RectF;

/**
 * @ingroup base
 * @brief A typedef to Rect with double values
 */
typedef Rect<double> RectD;

} // namespace base
} // namespace wiztk

#endif // WIZTK_CORE_RECT_HPP_
