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

#ifndef WIZTK_GUI_BUFFER_HPP_
#define WIZTK_GUI_BUFFER_HPP_

#include "wiztk/base/point.hpp"
#include "wiztk/base/size.hpp"
#include "wiztk/base/sigcxx.hpp"
#include "wiztk/base/macros.hpp"

#include <cstdint>
#include <memory>

namespace wiztk {
namespace gui {

class SharedMemoryPool;

/**
 * @brief Buffer in wayland compositor
 *
 * This class represents a native wayland buffer in memory. Use the Setup() to
 * allocate this buffer and should allocate again if the shm pool is reset.
 */
class Buffer {

  friend class Surface;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Buffer);

  using Point = base::PointI;
  using Size = base::SizeI;
  template<typename ... ParamTypes> using SignalRefT = typename base::SignalRefT<ParamTypes...>;
  template<typename ... ParamTypes> using SignalT = typename base::SignalT<ParamTypes...>;

  Buffer();

  ~Buffer();

  void Setup(const SharedMemoryPool &pool,
             int32_t width,
             int32_t height,
             int32_t stride,
             uint32_t format,
             int offset = 0);

  void Destroy();

  void SetPosition(int x, int y);

  const void *GetData() const;

  int32_t GetStride() const;

  int GetOffset() const;

  const Point &GetPosition() const;

  const Size &GetSize() const;

  int GetWidth() const;

  int GetHeight() const;

  SignalRefT<> release() { return release_; }

  SignalRefT<> destroyed() { return destroyed_; }

 private:

  struct Private;

  std::unique_ptr<Private> p_;

  SignalT<> release_;

  SignalT<> destroyed_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_BUFFER_HPP_
