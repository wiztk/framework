//

#ifndef WIZTK_TEST_SYSTEM_THREADING_THREAD_HPP_
#define WIZTK_TEST_SYSTEM_THREADING_THREAD_HPP_

#include <gtest/gtest.h>

class TestThread : public testing::Test {

 public:

  TestThread() = default;

  ~TestThread() override = default;

 protected:

  void SetUp() final {}

  void TearDown() final {}

};

#endif // WIZTK_TEST_SYSTEM_THREADING_THREAD_HPP_
