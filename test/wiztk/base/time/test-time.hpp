//

#ifndef WIZTK_TEST_BASE_MESSAGE_LOOP_HPP_
#define WIZTK_TEST_BASE_MESSAGE_LOOP_HPP_

#include <gtest/gtest.h>

class TestTime : public testing::Test {

 public:

  TestTime() = default;

  ~TestTime() override = default;

 protected:

  void SetUp() final {}

  void TearDown() final {}

};

#endif // WIZTK_TEST_BASE_MESSAGE_LOOP_HPP_
