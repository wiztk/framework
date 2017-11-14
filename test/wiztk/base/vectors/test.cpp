//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <wiztk/base/vector.hpp>

using namespace wiztk::base;
typedef Vector2T<int> vec2i;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, Vector2_1) {
  vec2i v1 {1, 1};
  vec2i v2 {2, 2};
  vec2i v3 {1, 1};

  ASSERT_TRUE(v1 == v3 && v1 != v2);
}

TEST_F(Test, Vector2_2) {
  vec2i v1 {1, 1};
  vec2i v2 {2, 2};

  vec2i v3 = v1 + v2;

  ASSERT_TRUE(v3.x == 3 && v3.y == 3);
}
