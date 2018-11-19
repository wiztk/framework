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

#ifndef WIZTK_GUI_SHARED_MEMORY_POOL_PRIVATE_HPP
#define WIZTK_GUI_SHARED_MEMORY_POOL_PRIVATE_HPP

#include "wiztk/gui/shared-memory-pool.hpp"

#include <wayland-client.h>

namespace wiztk {
namespace gui {

struct SharedMemoryPool::Private {

  static const Private &Get(const SharedMemoryPool &pool) {
    return *pool.p_;
  }

  static int CreateAnonymousFile(off_t size);

  static int CreateTempFile(char *tmpname);

  Private() = default;

  ~Private() {
    if (wl_shm_pool) wl_shm_pool_destroy(wl_shm_pool);
  }

  struct wl_shm_pool *wl_shm_pool = nullptr;

  int32_t size = 0;

  void *data = nullptr;

};

}
}

#endif // WIZTK_GUI_SHARED_MEMORY_POOL_PRIVATE_HPP
