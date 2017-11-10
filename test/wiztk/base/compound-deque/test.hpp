//
// Created by zhanggyb on 16-9-19.
//

#ifndef SKLAND_TEST_CORE_SIGCXX_TRACKABLE_HPP_
#define SKLAND_TEST_CORE_SIGCXX_TRACKABLE_HPP_

#include <gtest/gtest.h>

class Test : public testing::Test {
 public:
  Test();
  virtual ~Test();

 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};

#endif //WAYLAND_TOOLKIT_TEST_HPP
