//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <wiztk/base/thickness.hpp>

using Margin = wiztk::base::ThicknessI;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, constructor_1) {
  Margin m;

  ASSERT_TRUE(m.left == 0 && m.right == 0 && m.top == 0 && m.bottom == 0);
}

TEST_F(Test, constructor_2) {
  Margin m(1, 2, 3, 4);

  ASSERT_TRUE(m.left == 1 && m.top == 2 && m.right == 3 && m.bottom == 4);
}

TEST_F(Test, compare_1) {
  Margin m1(1, 2, 3, 4);
  Margin m2(1, 2, 3, 4);

  ASSERT_TRUE(m1 == m2);
}

TEST_F(Test, compare_2) {
  Margin m1(1, 2, 3, 4);
  Margin m2;

  ASSERT_TRUE(m1 != m2);
}
