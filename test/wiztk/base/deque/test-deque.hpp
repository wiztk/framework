//
// Created by zhanggyb on 16-9-19.
//

#ifndef SKLAND_TEST_BASE_COUNTED_DEQUE_HPP_
#define SKLAND_TEST_BASE_COUNTED_DEQUE_HPP_

#include <gtest/gtest.h>

class Test : public testing::Test {

 public:

  Test() = default;

  ~Test() override = default;

 protected:

  void SetUp() final {}

  void TearDown() final {}

};

#endif //WAYLAND_TOOLKIT_TEST_HPP
