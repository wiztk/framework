//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <iostream>
#include "wiztk/base/string.hpp"

using namespace wiztk;
using namespace wiztk::base;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, constructor_1) {

  ASSERT_TRUE(true);
}
