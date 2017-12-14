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

#include "wiztk/net/ip-address.hpp"

#include "wiztk/base/string.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

namespace wiztk {
namespace net {

std::unique_ptr<IPAddressList> IPAddress::GetByName(const char *name) {
  std::unique_ptr<IPAddressList> addr_list(new IPAddressList);

  struct hostent *host = gethostbyname(name);
  if (nullptr == host)
    throw std::runtime_error("Error! Fail to get host by name!");

  char **pp = nullptr;

  switch (host->h_addrtype) {
    case AF_INET: {
      IPv4Address *obj = nullptr;
      struct sockaddr_in *addr = nullptr;
      for (pp = host->h_addr_list; *pp != nullptr; ++pp) {
        obj = new IPv4Address;
        addr = reinterpret_cast<struct sockaddr_in *>(obj->address_);
        memcpy(&addr->sin_addr, *pp, static_cast<size_t >(host->h_length));
        addr_list->deque_.PushBack(obj);
      }
      break;
    }
    case AF_INET6: {
      IPv6Address *obj = nullptr;
      struct sockaddr_in6 *addr = nullptr;
      for (pp = host->h_addr_list; *pp != nullptr; ++pp) {
        obj = new IPv6Address;
        addr = reinterpret_cast<struct sockaddr_in6 *>(obj->address_);
        memcpy(&addr->sin6_addr, *pp, static_cast<size_t >(host->h_length));
        addr_list->deque_.PushBack(obj);
      }
      break;
    }
    default:break;
  }

  return addr_list;
}

IPAddress::~IPAddress() {
  if (nullptr != address_) {
    switch (address_->sa_family) {
      case AF_INET: {
        // IPv4
        auto *ipv4_address = reinterpret_cast<struct sockaddr_in *>(address_);
        delete ipv4_address;
        break;
      }
      case AF_INET6: {
        // IPv6
        auto *ipv6_address = reinterpret_cast<struct sockaddr_in6 *>(address_);
        delete ipv6_address;
        break;
      }
      default: {
        // TODO: check the address type
        delete address_;
        break;
      }
    }

    address_ = nullptr;
  }
}

AddressFamily IPAddress::GetAddressFamily() const {
  AddressFamily addr_family = kAddressFamilyUnspecified;
  if (nullptr != address_) {
    switch (address_->sa_family) {
      case AF_INET: {
        addr_family = kAddressFamilyINET;
        break;
      }
      case AF_INET6: {
        addr_family = kAddressFamilyINET6;
        break;
      }
      default:break;
    }
  }

  return addr_family;
}

std::string IPAddress::ToString() const {
  std::string str;

  if (nullptr == address_) return str;

  switch (address_->sa_family) {
    case AF_INET: {
      auto *addr = reinterpret_cast<struct sockaddr_in *>(address_);
      char buf[INET_ADDRSTRLEN];
      str = inet_ntop(AF_INET, &addr->sin_addr, buf, INET_ADDRSTRLEN);
      break;
    }
    case AF_INET6: {
      auto *addr = reinterpret_cast<struct sockaddr_in6 *>(address_);
      char buf[INET6_ADDRSTRLEN];
      str = inet_ntop(AF_INET6, &addr->sin6_addr, buf, INET6_ADDRSTRLEN);
      break;
    }
    default: break;
  }

  return str;
}

// -------------

IPv4Address::IPv4Address() {
  auto *addr = new sockaddr_in;

  bzero(addr, sizeof(struct sockaddr_in));
  addr->sin_family = AF_INET;

  address_ = reinterpret_cast<struct sockaddr *>(addr);
}

IPv6Address::IPv6Address() {
  auto *addr = new sockaddr_in6;

  bzero(addr, sizeof(struct sockaddr_in6));
  addr->sin6_family = AF_INET6;

  address_ = reinterpret_cast<struct sockaddr *>(addr);
}

} // namespace net
} // namespace wiztk
