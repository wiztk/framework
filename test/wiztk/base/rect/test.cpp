//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <wiztk/base/rect.hpp>

#include "SkRect.h"

using namespace wiztk;
using namespace wiztk::base;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, constructor_1) {
  RectF r;

  ASSERT_TRUE(r.x() == 0 && r.y() == 0 && r.width() == 0 && r.height() == 0);
}

TEST_F(Test, constructor_2) {
  RectF r(0, 0, 4, 4);

  ASSERT_TRUE(r.x() == 0 && r.y() == 0 && r.width() == 4 && r.height() == 4);
}

TEST_F(Test, MoveTo_1) {
  RectF rect = {10, 10, 20, 20};

  rect.MoveTo(20, 20);

  ASSERT_TRUE(rect.l == 20 && rect.t == 20 && rect.r == 30 && rect.b == 30);
}

TEST_F(Test, set_size_1) {
  RectF r;

  r.Resize(4, 4);

  ASSERT_TRUE(r.x() == 0 && r.y() == 0 && r.width() == 4 && r.height() == 4);
}

TEST_F(Test, set_position_1) {
  RectF r(0, 0, 4, 4);

  r.MoveTo(1, 1);

  ASSERT_TRUE(r.x() == 1 && r.y() == 1 && r.width() == 4 && r.height() == 4);
}

TEST_F(Test, set_position_2) {
  RectF r(0, 0, 4, 4);

  r.MoveTo(1, 1);

  ASSERT_TRUE(r.x() == 1 && r.y() == 1 && r.width() == 4 && r.height() == 4);
}

TEST_F(Test, contains_1) {
  RectF r(0, 0, 4, 4);

  ASSERT_TRUE(r.Contain(1, 1));
}

TEST_F(Test, contains_2) {
  RectF r(0, 0, 4, 4);

  ASSERT_TRUE(r.Contain(0, 0));
}

TEST_F(Test, contains_3) {
  RectF r(0, 0, 4, 4);

  ASSERT_TRUE(!r.Contain(4, 4));
}

TEST_F(Test, intersect_1) {
  RectF r1(0, 0, 10, 10);
  RectF r2(2, 2, 6, 6);

  ASSERT_TRUE(r1.Intersect(r2));
}

TEST_F(Test, intersect_2) {
  RectF r1(0, 0, 10, 10);
  RectF r2(11, 11, 18, 18);

  ASSERT_TRUE(!r1.Intersect(r2));
}

TEST_F(Test, intersect_3) {
  RectF r1(0, 0, 10, 10);
  RectF r2(-2, -2, 18, 18);

  ASSERT_TRUE(r1.Intersect(r2));
}

TEST_F(Test, intersect_4) {
  RectF r1(0, 0, 10, 10);
  RectF r2(9, 9, 18, 18);

  ASSERT_TRUE(r1.Intersect(r2));
}

TEST_F(Test, intersect_5) {
  RectF r1(0, 0, 10, 10);
  RectF r2(-9, -4, 0, 0);

  ASSERT_TRUE(!r1.Intersect(r2));
}

TEST_F(Test, hcenter_1) {
  RectF r(0, 0, 4, 4);

  ASSERT_TRUE(r.center_x() == 2);
}

TEST_F(Test, vcenter_1) {
  RectF r(0, 0, 4, 4);

  ASSERT_TRUE(r.center_y() == 2);
}

TEST_F(Test, cast_1) {
  RectF r(0.f, 0.f, 4.f, 4.f);
  SkRect* sk_rect = reinterpret_cast<SkRect*>(&r);

  ASSERT_TRUE(r.l == sk_rect->fLeft && r.r == sk_rect->fRight && r.t == sk_rect->fTop && r.b == sk_rect->fBottom);
}