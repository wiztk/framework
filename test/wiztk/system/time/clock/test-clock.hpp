//

#ifndef WIZTK_TEST_SYSTEM_TIME_CLOCK_HPP_
#define WIZTK_TEST_SYSTEM_TIME_CLOCK_HPP_

#include <gtest/gtest.h>

class TestClock : public testing::Test {

 public:

  TestClock() = default;

  ~TestClock() override = default;

 protected:

  void SetUp() final {}

  void TearDown() final {}

};

#endif // WIZTK_TEST_SYSTEM_TIME_CLOCK_HPP_
