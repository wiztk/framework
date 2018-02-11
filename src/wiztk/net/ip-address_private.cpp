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

#include "ip-address_private.hpp"

#include <cstring>

namespace wiztk {
namespace net {

IPAddress::Private::Private(const Private &other) {
  Copy(other);
}

IPAddress::Private::~Private() {
  Reset();
}

IPAddress::Private &IPAddress::Private::operator=(const Private &other) {
  Reset();
  Copy(other);
  return *this;
}

void IPAddress::Private::Reset() {
  if (nullptr == socket_address) return;;

  switch (socket_address->sa_family) {
    case AF_INET: {
      // IPv4
      auto *ipv4_address = reinterpret_cast<struct sockaddr_in *>(socket_address);
      delete ipv4_address;
      break;
    }
    case AF_INET6: {
      // IPv6
      auto *ipv6_address = reinterpret_cast<struct sockaddr_in6 *>(socket_address);
      delete ipv6_address;
      break;
    }
    default: {
      // TODO: check the address type
      delete socket_address;
      break;
    }
  }

  socket_address = nullptr;
}

void IPAddress::Private::Copy(const Private &other) {
  Reset();

  if (nullptr != other.socket_address) {
    switch (other.socket_address->sa_family) {
      case AF_INET: {
        auto *addr = new struct sockaddr_in;
        memcpy(addr,
               reinterpret_cast<struct sockaddr_in *>(other.socket_address),
               sizeof(struct sockaddr_in));
        socket_address = reinterpret_cast<struct sockaddr *>(addr);
        break;
      }
      case AF_INET6: {
        auto *addr = new struct sockaddr_in6;
        memcpy(addr,
               reinterpret_cast<struct sockaddr_in6 *>(other.socket_address),
               sizeof(struct sockaddr_in6));
        socket_address = reinterpret_cast<struct sockaddr *>(addr);
        break;
      }
      default:break;
    }
  }
}

}
}