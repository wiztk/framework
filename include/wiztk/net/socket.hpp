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

#include "wiztk/base/string.hpp"

#include "wiztk/net/ip-address.hpp"

#include <sys/types.h>
#include <sys/socket.h>

namespace wiztk {
namespace net {

// Forward declaration:
class IPAddress;
class Proxy;

/**
 * @ingroup net
 * @brief This class implements client socket.
 *
 * A socket is an endpoint for communication between two machines.
 */
class Socket {

 public:

  using String = base::String;

  /**
   * @brief Default constructor.
   *
   * Creates an unconnected socket.
   */
  Socket();

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

  void Bind();

  void Close();

  void Connect(const IPAddress &address);

  void Connect(const IPAddress &address, int timeout);

 private:

  std::unique_ptr<IPAddress> adress_;

  int socket_;

};

} // namespace net
} // namespace wiztk

#endif // WIZTK_NET_SOCKET_HPP_
