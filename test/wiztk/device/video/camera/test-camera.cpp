//
// Created by zhanggyb on 16-9-19.
//

#include "test-camera.hpp"

#include <wiztk/device/video/camera.hpp>

using namespace wiztk;
using namespace wiztk::device;

TEST_F(TestCamera, query_all_1) {
  video::Camera::GetAll();

  ASSERT_TRUE(true);
}