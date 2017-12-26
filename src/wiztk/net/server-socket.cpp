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

#include "wiztk/net/server-socket.hpp"

#include "wiztk/net/socket-exception.hpp"
#include "wiztk/net/io-buffer.hpp"

#include "internal/ip-address_native.hpp"

#include <unistd.h>

namespace wiztk {
namespace net {

ServerSocket::ServerSocket(AddressFamily address_family,
                           SocketType socket_type,
                           ProtocolType protocol_type) {
  socket_ = socket(address_family, socket_type, protocol_type);

  if (socket_ < 0) throw SocketException("Error! Fail to initialize a new ServerSocket object!");
}

ServerSocket::~ServerSocket() {
  if (socket_)
    close(socket_);
}

void ServerSocket::Bind(const IPAddress &address) const {
  using _ = IPAddress::Native;

  int ret = bind(socket_, _::GetSocketAddress(address), _::GetLength(address));
  if (ret < 0)
    throw SocketException("Error! Cannot bind this socket!");
}

void ServerSocket::Listen(int backlog) const {
  int ret = listen(socket_, backlog);

  if (ret < 0)
    throw SocketException("Error! Cannot listen this socket!");
}

void ServerSocket::Accept() const {
  int ret = accept(socket_, nullptr, nullptr);

  if (ret < 0)
    throw SocketException("Error! Cannot accept this socket!");
}

size_t ServerSocket::Send(const IOBuffer &buffer, size_t size) {
  ssize_t ret = send(socket_, buffer.data(), size, 0);
  if (ret < 0)
    throw SocketException("Error! Cannot send data to socket!");

  return static_cast<size_t>(ret);
}

size_t ServerSocket::Receive(IOBuffer &buffer, size_t size) {
  ssize_t ret = recv(socket_, buffer.data(), size, 0);
  if (ret < 0)
    throw SocketException("Error! Cannot receive data from socket!");

  return static_cast<size_t>(ret);
}

void ServerSocket::Close() {
  if (socket_) {
    close(socket_);
    socket_ = 0;
  }
}

} // namespace net
} // namespace wiztk
