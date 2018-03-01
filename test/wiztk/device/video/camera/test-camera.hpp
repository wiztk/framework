//
// Created by zhanggyb on 16-9-19.
//

#ifndef WIZTK_TEST_DEVICE_VIDEO_CAMERA_HPP_
#define WIZTK_TEST_DEVICE_VIDEO_CAMERA_HPP_

#include <gtest/gtest.h>

class TestCamera : public testing::Test {

 public:

  TestCamera() = default;

  ~TestCamera() override = default;

 protected:

  void SetUp() final {}

  void TearDown() final {}

};

#endif // WIZTK_TEST_DEVICE_VIDEO_CAMERA_HPP_
