//
// Created by zhanggyb on 16-9-19.
//

#ifndef WIZTK_TEST_BASE_MEMORY_REF_COUNTED_BASE_HPP_
#define WIZTK_TEST_BASE_MEMORY_REF_COUNTED_BASE_HPP_

#include <gtest/gtest.h>

class TestColor : public testing::Test {

 public:

  TestColor() = default;

  ~TestColor() override = default;

 protected:

  void SetUp() final {}

  void TearDown() final {}

};

#endif //WAYLAND_TOOLKIT_TEST_HPP
