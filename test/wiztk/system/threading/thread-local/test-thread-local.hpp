//

#ifndef WIZTK_TEST_SYSTEM_THREADING_THREAD_LOCAL_HPP_
#define WIZTK_TEST_SYSTEM_THREADING_THREAD_LOCAL_HPP_

#include <gtest/gtest.h>

class TestThreadLocal : public testing::Test {

 public:

  TestThreadLocal() = default;

  ~TestThreadLocal() override = default;

 protected:

  void SetUp() final {}

  void TearDown() final {}

};

#endif // WIZTK_TEST_SYSTEM_THREADING_THREAD_LOCAL_HPP_
