//
// Created by zhanggyb on 16-9-19.
//

#ifndef WIZTK_TEST_BASE_MEMORY_REF_COUNTED_BASE_HPP_
#define WIZTK_TEST_BASE_MEMORY_REF_COUNTED_BASE_HPP_

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
