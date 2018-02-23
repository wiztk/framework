//

#ifndef WIZTK_TEST_SYSTEM_LOGGING_LOGGER_HPP_
#define WIZTK_TEST_SYSTEM_LOGGING_LOGGER_HPP_

#include <gtest/gtest.h>

class TestLogger : public testing::Test {

 public:

  TestLogger() = default;

  ~TestLogger() override = default;

 protected:

  void SetUp() final {}

  void TearDown() final {}

};

#endif // WIZTK_TEST_SYSTEM_LOGGING_LOGGER_HPP_
