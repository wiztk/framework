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

#ifndef WIZTK_BASE_COLOR_HPP_
#define WIZTK_BASE_COLOR_HPP_

#include "wiztk/base/clamp.hpp"

#include <cstdint>
#include <cstring>

namespace wiztk {
namespace base {

/**
 * @ingroup base
 * @brief Template class for color
 * @tparam T Only support float or double
 */
template<typename T>
struct Color {

  inline static uint32_t GetAlpha(uint32_t argb) {
    return (argb >> 24) & 0xFF;
  }

  inline static uint32_t GetRed(uint32_t argb) {
    return (argb >> 16) & 0xFF;
  }

  inline static uint32_t GetGreen(uint32_t argb) {
    return (argb >> 8) & 0xFF;
  }

  inline static uint32_t GetBlue(uint32_t argb) {
    return (argb >> 0) & 0xFF;
  }

  inline static uint32_t SetAlpha(uint32_t argb, unsigned char alpha) {
    return (argb & 0x00FFFFFF) | (alpha << 24);
  }

  inline static Color FromRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
    return Color(r / T(255.), g / T(255.), b / T(255.), a / T(255.));
  }

  inline static Color ReverseRGBFrom(const Color &color) {
    Color ret = color;
    ret.ReverseRGB();
    return ret;
  }

  Color()
      : r(T(0)), g(T(0)), b(T(0)), a(T(1.)) {
  }

  Color(T r, T g, T b, T a = T(1.))
      : r(r), g(g), b(b), a(a) {
  }

  Color(uint32_t argb) {
    a = ((argb >> 24) & 0xFF) / T(255.);
    r = ((argb >> 16) & 0xFF) / T(255.);
    g = ((argb >> 8) & 0xFF) / T(255.);
    b = ((argb >> 0) & 0xFF) / T(255.);
  }

  Color(const Color &other)
      : r(other.r), g(other.g), b(other.b), a(other.a) {}

  Color &operator=(const Color &other) {
    r = other.r;
    g = other.g;
    b = other.b;
    a = other.a;
    return *this;
  }

  Color &operator=(uint32_t argb) {
    a = ((argb >> 24) & 0xFF) / T(255.);
    r = ((argb >> 16) & 0xFF) / T(255.);
    g = ((argb >> 8) & 0xFF) / T(255.);
    b = ((argb >> 0) & 0xFF) / T(255.);
    return *this;
  }

  Color &operator+=(short shade) {
    r = Clamp(r + shade / T(255.), T(0.), T(1.));
    g = Clamp(g + shade / T(255.), T(0.), T(1.));
    b = Clamp(b + shade / T(255.), T(0.), T(1.));

    return *this;
  }

  Color &operator-=(short shade) {
    r = Clamp(r - shade / T(255.), T(0.), T(1.));
    g = Clamp(g - shade / T(255.), T(0.), T(1.));
    b = Clamp(b - shade / T(255.), T(0.), T(1.));

    return *this;
  }

  void ReverseRGB() {
    r = T(1.) - r;
    g = T(1.) - g;
    b = T(1.) - b;
  }

  void ReverseRGBA() {
    r = T(1.) - r;
    g = T(1.) - g;
    b = T(1.) - b;
    a = T(1.) - a;
  }

  uint32_t argb() const {
    return (uchar_alpha() << 24) | (uchar_red() << 16) | (uchar_green() << 8) | (uchar_blue());
  }

  uint32_t rgba() const {
    return (uchar_red() << 24) | (uchar_green() << 16) | (uchar_blue() << 8) | (uchar_alpha());
  }

  unsigned char uchar_red() const {
    return (unsigned char) (r * 255);
  }

  unsigned char uchar_green() const {
    return (unsigned char) (g * 255);
  }

  unsigned char uchar_blue() const {
    return (unsigned char) (b * 255);
  }

  unsigned char uchar_alpha() const {
    return (unsigned char) (a * 255);
  }

  union { T r, red; };
  union { T g, green; };
  union { T b, blue; };
  union { T a, alpha; };

};

template<typename T>
inline Color<T> operator+(const Color<T> &src, short shade) {
  Color<T> color;
  color.r = Clamp(src.r + shade / T(255.), T(0.), T(1.));
  color.g = Clamp(src.g + shade / T(255.), T(0.), T(1.));
  color.b = Clamp(src.b + shade / T(255.), T(0.), T(1.));
  color.a = src.a;
  return color;
}

template<typename T>
inline Color<T> operator+(const Color<T> &color1, const Color<T> &color2) {
  Color<T> color;
  color.r = Clamp(color1.r + color2.r, T(0.), T(1.));
  color.g = Clamp(color1.g + color2.g, T(0.), T(1.));
  color.b = Clamp(color1.b + color2.b, T(0.), T(1.));
  color.a = Clamp(color1.a + color2.a, T(0.), T(1.));
  return color;
}

template<typename T>
inline Color<T> operator-(const Color<T> &src, short shade) {
  Color<T> color;
  color.r = Clamp(src.r - shade / T(255.), T(0.), T(1.));
  color.g = Clamp(src.g - shade / T(255.), T(0.), T(1.));
  color.b = Clamp(src.b - shade / T(255.), T(0.), T(1.));
  color.a = src.a;

  return color;
}

template<typename T>
inline Color<T> operator-(const Color<T> &color1, const Color<T> &color2) {
  Color<T> color;
  color.r = Clamp(color1.r - color2.r, T(0.), T(1.));
  color.g = Clamp(color1.g - color2.g, T(0.), T(1.));
  color.b = Clamp(color1.b - color2.b, T(0.), T(1.));
  color.a = Clamp(color1.a - color2.a, T(0.), T(1.));

  return color;
}

template<typename T>
inline bool operator==(const Color<T> &src, const Color<T> &dst) {
  return memcmp(&src, &dst, sizeof(Color<T>)) == 0;
}

template<typename T>
inline bool operator!=(const Color<T> &src, const Color<T> &dst) {
  return memcmp(&src, &dst, sizeof(Color<T>)) != 0;
}

/**
 * @ingroup base
 * @brief A typedef to color with float values
 */
typedef Color<float> ColorF;

/**
 * @ingroup base
 * @brief A typedef to color with double values
 */
typedef Color<double> ColorD;

} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_COLOR_HPP_
