//
// Created by zhanggyb on 16-9-19.
//

#ifndef SKLAND_TEST_CORE_SIGCXX_TRACKABLE_HPP_
#define SKLAND_TEST_CORE_SIGCXX_TRACKABLE_HPP_

#include <gtest/gtest.h>

class MatrixTest : public testing::Test {
 public:
  MatrixTest();
  virtual ~MatrixTest();

 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};

#endif //WAYLAND_TOOLKIT_TEST_HPP
