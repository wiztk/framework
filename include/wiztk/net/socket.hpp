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

#ifndef WIZTK_NET_SOCKET_HPP_
#define WIZTK_NET_SOCKET_HPP_

#include "wiztk/base/macros.hpp"
#include "wiztk/base/string.hpp"

#include "wiztk/net/address-family.hpp"
#include "wiztk/net/socket-type.hpp"
#include "wiztk/net/protocol-type.hpp"

namespace wiztk {
namespace net {

// Forward declarations:
class IPAddress;
class Proxy;
class IOBuffer;

/**
 * @ingroup net
 * @brief Client-side socket.
 *
 * A socket is an endpoint for communication between two machines.
 */
class WIZTK_EXPORT Socket {

 public:

  using String = base::String;

  /**
   * @brief Default constructor.
   *
   * Creates an unconnected socket.
   */
  Socket() = default;

  /**
   * @brief Constructor which creates an endpoint for communication.
   * @param address_family
   * @param socket_type
   * @param protocol_type
   */
  Socket(AddressFamily address_family,
         SocketType socket_type,
         ProtocolType protocol_type = kProtocolDefault);

  /**
   * @brief Creates a stream socket and connects it to the specified port number on the named host.
   * @param host
   * @param port
   */
  Socket(const String &host, int port);

  /**
   * @brief Creates a socket and connects it to the specified remote host on the specified remote port.
   * @param host
   * @param port
   * @param local_address
   * @param local_port
   */
  Socket(const String &host, int port, const IPAddress &local_address, int local_port);

  /**
   * @brief Creates a stream socket and connects it to the specified port number at the specified IP address.
   * @param address
   * @param port
   */
  Socket(const IPAddress &address, int port);

  Socket(const IPAddress &address, int port, const IPAddress &local_address, int local_port);

  Socket(const Proxy &proxy);

  ~Socket();

  void Connect(const IPAddress &address);

  void Connect(const IPAddress &address, int timeout);

  size_t Send(const IOBuffer &buffer, size_t buffer_size);

  size_t Receive(IOBuffer &buffer, size_t buffer_size);

  void Close();

  AddressFamily GetAddressFamily() const;

 private:

  int socket_ = 0;

};

} // namespace net
} // namespace wiztk

#endif // WIZTK_NET_SOCKET_HPP_
