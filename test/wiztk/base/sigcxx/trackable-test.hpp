#ifndef WIZTK_TEST_BASE_SIGCXX_TRACKABLE_HPP_
#define WIZTK_TEST_BASE_SIGCXX_TRACKABLE_HPP_

#include <gtest/gtest.h>

class TrackableTest : public testing::Test {

 public:

  TrackableTest() = default;

  ~TrackableTest() override = default;

 protected:

  void SetUp() final {}

  void TearDown() final {}

};

#endif // WIZTK_TEST_BASE_SIGCXX_TRACKABLE_HPP_
