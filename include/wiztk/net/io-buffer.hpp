/*
 * Copyright 2017 The WizTK Authors.
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

#ifndef WIZTK_NET_IO_BUFFER_HPP_
#define WIZTK_NET_IO_BUFFER_HPP_

#include <cstddef>

namespace wiztk {
namespace net {

/**
 * @ingroup net
 * @brief Buffer for socket read/write.
 */
class IOBuffer {

 public:

  /**
   * @brief Default constructor.
   */
  IOBuffer() = default;

  /**
   * @brief Constructor by given size.
   * @param size
   */
  explicit IOBuffer(size_t size);

  /**
   * @brief Move constructor.
   * @param other
   */
  IOBuffer(IOBuffer &&other) noexcept
      : data_(other.data_) {
    other.data_ = nullptr;
  }

  /**
   * @brief Destructor.
   */
  ~IOBuffer();

  /**
   * @brief Move operator.
   * @param other
   * @return
   */
  IOBuffer &operator=(IOBuffer &&other) noexcept {
    data_ = other.data_;
    other.data_ = nullptr;
    return *this;
  }

  void Reset(size_t size);

  char *data() const { return data_; }

 private:

  char *data_ = nullptr;

};

} // namespace net
} // namespace wiztk

#endif //WIZTK_IO_BUFFER_HPP
