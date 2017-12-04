//
// Created by zhanggyb on 16-9-19.
//

#ifndef WIZTK_TEST_BASE_MEMORY_REF_COUNTED_BASE_HPP_
#define WIZTK_TEST_BASE_MEMORY_REF_COUNTED_BASE_HPP_

#include <gtest/gtest.h>

class TestString : public testing::Test {

 public:

  TestString() = default;

  ~TestString() override = default;

 protected:

  void SetUp() override {}

  void TearDown() override {}

};

#endif //WAYLAND_TOOLKIT_TEST_HPP
