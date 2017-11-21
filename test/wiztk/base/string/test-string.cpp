//
// Created by zhanggyb on 16-9-19.
//

#include "test-string.hpp"

#include <iostream>
#include "wiztk/base/string.hpp"

#include <unicode/unistr.h>

using namespace wiztk;
using namespace wiztk::base;

TEST_F(TestString, print_1) {

  String16 str16(u"汉字");

  String32 str32(U"汉字");

  std::cout << "Print: " << str16 << std::endl;
  std::cout << "Print: " << str32 << std::endl;

  ASSERT_TRUE(true);
}
