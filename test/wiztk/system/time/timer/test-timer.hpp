//
// Created by zhanggyb on 16-9-19.
//

#ifndef WIZTK_TEST_SYSTEM_TIME_TIMER_HPP_
#define WIZTK_TEST_SYSTEM_TIME_TIMER_HPP_

#include <gtest/gtest.h>

class TestTimer : public testing::Test {

 public:

  TestTimer() = default;

  ~TestTimer() override = default;

 protected:

  void SetUp() final {}

  void TearDown() final {}

};

#endif // WIZTK_TEST_SYSTEM_TIME_TIMER_HPP_
