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

#ifndef WIZTK_NET_SERVER_SOCKET_HPP_
#define WIZTK_NET_SERVER_SOCKET_HPP_

#include "wiztk/base/macros.hpp"

#include "wiztk/net/address-family.hpp"
#include "wiztk/net/socket-type.hpp"
#include "wiztk/net/protocol-type.hpp"

namespace wiztk {
namespace net {

// Forward declaration:
class IPAddress;
class Proxy;
class IOBuffer;

/**
 * @ingroup net
 * @brief Server-side socket.
 */
class WIZTK_EXPORT ServerSocket {

 public:

  ServerSocket() = default;

  ServerSocket(AddressFamily address_family,
               SocketType socket_type,
               ProtocolType protocol_type = kProtocolDefault);

  ~ServerSocket();

  void Bind(const IPAddress &address) const;

  void Listen(int backlog) const;

  void Accept() const;

  size_t Send(const IOBuffer &buf, size_t size);

  size_t Receive(IOBuffer &buf, size_t size);

  void Close();

 private:

  int socket_ = 0;

};

}  // namespace net
}  // namespace wiztk

#endif  // WIZTK_NET_SERVER_SOCKET_HPP_
