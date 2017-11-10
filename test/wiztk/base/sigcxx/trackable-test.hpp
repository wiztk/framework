//
// Created by zhanggyb on 16-9-19.
//

#ifndef SKLAND_TEST_CORE_SIGCXX_TRACKABLE_HPP_
#define SKLAND_TEST_CORE_SIGCXX_TRACKABLE_HPP_

#include <gtest/gtest.h>

class TrackableTest : public testing::Test {

 public:

  TrackableTest() {}

  virtual ~TrackableTest() {}

 protected:

  virtual void SetUp() {}

  virtual void TearDown() {}

};

#endif // SKLAND_TEST_CORE_SIGCXX_TRACKABLE_HPP_
