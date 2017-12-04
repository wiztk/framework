/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
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

#include "internal/buffer_private.hpp"

#include "wiztk/gui/shared-memory-pool.hpp"

namespace wiztk {
namespace gui {

using Point = base::Point2I;
using Size = base::SizeI;

Buffer::Buffer() {
  p_ = std::make_unique<Private>();
}

Buffer::~Buffer() {
  Destroy();
}

void Buffer::Setup(const SharedMemoryPool &pool,
                   int32_t width,
                   int32_t height,
                   int32_t stride,
                   uint32_t format,
                   int offset) {
  Destroy();

  int size = stride * height;
  if ((offset + size) > pool.size()) {
    throw std::runtime_error("Error! Trying to allocate buffer on small SHM pool.");
  }

  p_->wl_buffer = wl_shm_pool_create_buffer(pool.wl_shm_pool_,
                                            offset,
                                            width,
                                            height,
                                            stride,
                                            format);
  wl_buffer_add_listener(p_->wl_buffer, &Private::kListener, this);
  p_->size.width = width;
  p_->size.height = height;
  p_->stride = stride;
  p_->format = format;
  p_->offset = offset;
  p_->data = (char *) pool.data() + offset;
}

void Buffer::Destroy() {
  if (nullptr != p_->wl_buffer) {
    p_->data = nullptr;
    p_->offset = 0;
    p_->format = 0;
    p_->stride = 0;
    p_->size.width = 0;
    p_->size.height = 0;
    wl_buffer_destroy(p_->wl_buffer);
    p_->wl_buffer = nullptr;

    destroyed_.Emit();
  }
}

void Buffer::SetPosition(int x, int y) {
  p_->position.x = x;
  p_->position.y = y;
}

const void *Buffer::GetData() const {
  return p_->data;
}

int32_t Buffer::GetStride() const {
  return p_->stride;
}

int Buffer::GetOffset() const {
  return p_->offset;
}

const Point &Buffer::GetPosition() const {
  return p_->position;
}

const Size &Buffer::GetSize() const {
  return p_->size;
}

int Buffer::GetWidth() const {
  return p_->size.width;
}

int Buffer::GetHeight() const {
  return p_->size.height;
}

} // namespace gui
} // namespace wiztk
