/*
 * Copyright 2017 - 2018 The WizTK Authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "wiztk/device/video/camera.hpp"

#include <string>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <linux/videodev2.h>
#include <sys/ioctl.h>

namespace wiztk {
namespace device {
namespace video {

void Camera::GetAll() {
  int fd = -1;

  char device_name[16];

  for (int i = 0; i < 64; ++i) {
    sprintf(device_name, "/dev/video%d", i);

    fd = OpenDevice(device_name);
    if (-1 == fd) break;

    ShowInfo(fd);

    close(fd);
  }
}

int Camera::OpenDevice(const char *dev_name) {
  int fd = -1;
  struct stat st = {0};

  if (-1 == stat(dev_name, &st)) {
    // Cannot get file status
    return fd;
  }

  if (!S_ISCHR(st.st_mode)) {
    // Not a device
    return fd;
  }

  fd = open(dev_name, O_RDWR | O_NONBLOCK, 0);

  return fd;
}

void Camera::ShowInfo(int fd) {
  struct v4l2_capability cap;

  int ret;
  do {
    ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
  } while (-1 == ret && EINTR == errno);

  fprintf(stdout, "Name of the driver: %s\n", cap.driver);
  fprintf(stdout, "Name of the card: %s\n", cap.card);
}

} // namespace video
} // namespace device
} // namespace wiztk
