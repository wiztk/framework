//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <wiztk/base/trace.hpp>

using namespace wiztk;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

class TestCase {

 public:

  TestCase() {}

  ~TestCase() {}

  void TestDepth1() {
    Trace trace(__PRETTY_FUNCTION__, "%s", "Test depth 1");
    TestDepth2();
  }

  void TestDepth2() {
    Trace trace(__PRETTY_FUNCTION__, "%s", "Test depth 2");
    TestDepth3();
  }

  void TestDepth3() {
    Trace trace(__PRETTY_FUNCTION__, "%s", "Test depth 3");
  }

};

TEST_F(Test, constructor_1) {
  TestCase t;
  t.TestDepth1();

  _TRACE("%s", "Another message");

  ASSERT_TRUE(true);
}
