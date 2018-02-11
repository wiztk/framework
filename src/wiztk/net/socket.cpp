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

#include "wiztk/net/socket.hpp"
#include "wiztk/net/socket-exception.hpp"
#include "wiztk/net/io-buffer.hpp"

#include "ip-address_native.hpp"

#include <unistd.h>

namespace wiztk {
namespace net {

Socket::Socket(AddressFamily address_family,
               SocketType socket_type,
               ProtocolType protocol_type) {
  socket_ = socket(address_family, socket_type, protocol_type);

  if (socket_ < 0) throw SocketException("Error! Fail to initialize a new Socket object!");
}

Socket::Socket(const String &host, int port)
    : Socket() {

}

Socket::~Socket() {
  if (socket_)
    close(socket_);
}

void Socket::Connect(const IPAddress &address) {
  using _ = IPAddress::Native;

  int ret = connect(socket_, _::GetSocketAddress(address), _::GetSocketLength(address));
  if (ret < 0) {
    throw SocketException("Error! Cannot connect this socket!");
  }
}

size_t Socket::Send(const IOBuffer &buffer, size_t buffer_size) {
  ssize_t ret = send(socket_, buffer.data(), buffer_size, 0);
  if (ret < 0)
    throw SocketException("Error! Cannot send data to socket!");

  return static_cast<size_t>(ret);
}

size_t Socket::Receive(IOBuffer &buffer, size_t buffer_size) {
  ssize_t ret = recv(socket_, buffer.data(), buffer_size, 0);
  if (ret < 0)
    throw SocketException("Error! Cannot receive data from socket!");

  return static_cast<size_t>(ret);
}

void Socket::Close() {
  if (socket_) {
    close(socket_);
    socket_ = 0;
  }
}

AddressFamily Socket::GetAddressFamily() const {
  struct sockaddr_storage buf = {0};
  socklen_t length;

  auto *addr = reinterpret_cast<struct sockaddr *>(&buf);

  int ret = getsockname(socket_, addr, &length);

  if (ret < 0) {
    throw SocketException("Error! Fail to get address family!");
  }

  return static_cast<AddressFamily>(addr->sa_family);
}

} // namespace net
} // namespace wiztk
