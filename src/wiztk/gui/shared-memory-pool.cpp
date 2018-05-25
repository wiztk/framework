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

#include "display/proxy.hpp"

#include <wiztk/gui/shared-memory-pool.hpp>
#include "wiztk/gui/application.hpp"

#include <sys/mman.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>

#define HAVE_POSIX_FALLOCATE
#define HAVE_MKOSTEMP

#ifdef HAVE_POSIX_FALLOCATE
#include <fcntl.h>
#include <new>
#endif

namespace wiztk {
namespace gui {

void SharedMemoryPool::Setup(int32_t size) {
  Destroy();

  int fd = CreateAnonymousFile(size);
  if (fd < 0) throw std::runtime_error("Cannot create anonymous file for SHM");

  data_ = mmap(NULL, (size_t) size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (data_ == MAP_FAILED) {
    _DEBUG("%s\n", "Fail to map pages of memory");
    data_ = nullptr;
    close(fd);
    throw std::runtime_error("Cannot map shared memory");
  }

  Display *display = Application::GetInstance()->GetDisplay();
  wl_shm_pool_ = wl_shm_create_pool(Display::Proxy::wl_shm(display), fd, size);

  size_ = size;
  close(fd);
}

void SharedMemoryPool::Destroy() {
  if (wl_shm_pool_) {
    _ASSERT(data_);

    if (munmap(data_, (size_t) size_))
      _DEBUG("%s\n", "Failed to unmap the memory\n");

    data_ = nullptr;
    size_ = 0;

    wl_shm_pool_destroy(wl_shm_pool_);
    wl_shm_pool_ = nullptr;
  }
}

int SharedMemoryPool::CreateAnonymousFile(off_t size) {
  static const char temp[] = "/wiztk-XXXXXX";
  const char *path;
  char *name;
  int fd;
  int ret;

  path = getenv("XDG_RUNTIME_DIR");
  if (!path) {
    errno = ENOENT;
    return -1;
  }

  name = (char *) malloc(strlen(path) + sizeof(temp));
  if (!name)
    return -1;

  strcpy(name, path);
  strcat(name, temp);

  fd = CreateTempFile(name);

  free(name);

  if (fd < 0)
    return -1;

#ifdef HAVE_POSIX_FALLOCATE
  ret = posix_fallocate(fd, 0, size);
  if (ret != 0) {
    close(fd);
    errno = ret;
    return -1;
  }
#else
  ret = ftruncate(fd, size);
  if (ret < 0) {
    close(fd);
    return -1;
  }
#endif

  return fd;
}

int SharedMemoryPool::CreateTempFile(char *tmpname) {
  int fd;

#ifdef HAVE_MKOSTEMP
  fd = mkostemp(tmpname, O_CLOEXEC);
  if (fd >= 0)
    unlink(tmpname);
#else
  fd = mkstemp(tmpname);
  if (fd >= 0) {
    fd = set_cloexec_or_close(fd);
    unlink(tmpname);
  }
#endif

  return fd;
}

} // namespace gui
} // namespace wiztk
