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

#include "shared-memory-pool/private.hpp"

#include "display/private.hpp"

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

SharedMemoryPool::SharedMemoryPool() {
  p_ = std::make_unique<Private>();
}

SharedMemoryPool::~SharedMemoryPool() = default;

void SharedMemoryPool::Setup(int32_t size) {
  Destroy();

  int fd = Private::CreateAnonymousFile(size);
  if (fd < 0) throw std::runtime_error("Cannot create anonymous file for SHM");

  p_->data = mmap(NULL, (size_t) size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (p_->data == MAP_FAILED) {
    _DEBUG("%s\n", "Fail to map pages of memory");
    p_->data = nullptr;
    close(fd);
    throw std::runtime_error("Cannot map shared memory");
  }

  Display *display = Application::GetInstance()->GetDisplay();
  p_->wl_shm_pool = wl_shm_create_pool(Display::Private::Get(*display).wl_shm, fd, size);

  p_->size = size;
  close(fd);
}

void SharedMemoryPool::Destroy() {
  if (p_->wl_shm_pool) {
    _ASSERT(p_->data);

    if (munmap(p_->data, (size_t) p_->size))
      _DEBUG("%s\n", "Failed to unmap the memory\n");

    p_->data = nullptr;
    p_->size = 0;

    wl_shm_pool_destroy(p_->wl_shm_pool);
    p_->wl_shm_pool = nullptr;
  }
}

int32_t SharedMemoryPool::GetSize() const {
  return p_->size;
}

void *SharedMemoryPool::GetData() const {
  return p_->data;
}

} // namespace gui
} // namespace wiztk
