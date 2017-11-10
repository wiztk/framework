//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <wiztk/base/color.hpp>

#include "SkColor.h"

using namespace wiztk;
using namespace wiztk::base;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, constructor_1) {
  ColorD c;

  ASSERT_TRUE(c.r == 0.0 && c.g == 0.0 && c.b == 0.0 && c.a == 1.0);
}

TEST_F(Test, constructor_2) {
  ColorD c(0xFF00FF);

  ASSERT_TRUE(c.r == 1.0 && c.g == 0.0 && c.b == 1.0 && c.a == 0.0);
}

TEST_F(Test, assign_1) {
  ColorD c;
  c = 0xFF00FF00;
  ASSERT_TRUE(c.r == 0.0 && c.g == 1.0 && c.b == 0.0 && c.a == 1.0);
}

TEST_F(Test, assign_2) {
  ColorD c1;
  c1 = 0xFF00FF00;
  ColorD c;
  c = c1;

  ASSERT_TRUE(c.r == 0.0 && c.g == 1.0 && c.b == 0.0 && c.a == 1.0);
}

TEST_F(Test, compare_1) {
  ColorD c1(0xFF);
  ColorD c2(0xFF);

  ASSERT_TRUE(c1 == c2);
}

TEST_F(Test, compare_2) {
  ColorD c1(0.4, 0.6, 0.2, 0.9);
  ColorD c2(0.8, 0.2, 0.5, 0.7);

  ASSERT_TRUE(c1 != c2);
}

TEST_F(Test, argb_1) {
  uint32_t argb = 0xFFEEDDCC;
  ColorD c(argb);

  ASSERT_TRUE(argb == c.argb());
}

TEST_F(Test, operator_1) {
  ColorF color1(0.f, 0.f, 0.f);
  ColorF color2 = color1 + 15;  // 15 / 255.f

  bool result = ((color2.r - 0.058823f) < 0.001) &&
      ((color2.g - 0.058823f) < 0.001) &&
      ((color2.b - 0.058823f) < 0.001);

  ASSERT_TRUE(result);
}

TEST_F(Test, operator_2) {
  uint32_t argb = 0xFF4E4E4E;
  ColorF color;
  color = argb;

  ASSERT_TRUE(argb == color.argb());
}

TEST_F(Test, cast_1) {
  ColorF c(1.f, 1.f, 1.f, 1.f);

  SkColor4f* sk_color = reinterpret_cast<SkColor4f*>(&c);

  ASSERT_TRUE((sk_color->fR == 1.f) &&
      (sk_color->fG == 1.f) &&
      (sk_color->fB == 1.f) &&
      (sk_color->fA == 1.f));
}