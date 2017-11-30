//

#ifndef WIZTK_TEST_BASE_DELEGATE_HPP_
#define WIZTK_TEST_BASE_DELEGATE_HPP_

#include <gtest/gtest.h>

class TestDelegate : public testing::Test {

 public:

  TestDelegate() = default;

  ~TestDelegate() override = default;

 protected:

  void SetUp() final {}

  void TearDown() final {}

};

#endif // WIZTK_TEST_BASE_DELEGATE_HPP_
