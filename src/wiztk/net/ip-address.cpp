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

#include "internal/ip-address_native.hpp"
#include "internal/address-info_private.hpp"

#include <arpa/inet.h>

#include <cstring>

namespace wiztk {
namespace net {

std::unique_ptr<IPAddressList> IPAddress::GetByHostAndService(const char *host,
                                                              const char *service,
                                                              const AddressInfo *hints) {
  std::unique_ptr<IPAddressList> list_ptr(new IPAddressList);

  struct addrinfo *addr_info = nullptr;

  int result = getaddrinfo(host,
                           service,
                           nullptr == hints ? nullptr : hints->p_->address_info,
                           &addr_info);

  if (result) {
    if (nullptr != addr_info) {
      freeaddrinfo(addr_info);
    }

    // TODO: check the return value

    throw std::runtime_error("Error! Cannot get address list!");
  }

  IPAddress *obj = nullptr;
  struct addrinfo *ai_ptr = addr_info;

  switch (ai_ptr->ai_family) {
    case AF_INET: {
      while (nullptr != ai_ptr) {
        obj = new IPAddress;
        auto *addr = new struct sockaddr_in;
        memcpy(addr, ai_ptr->ai_addr, ai_ptr->ai_addrlen);
        obj->p_->socket_address = reinterpret_cast<struct sockaddr *>(addr);
        list_ptr->deque_.push_back(obj->p_.get());
        ai_ptr = ai_ptr->ai_next;
      }
      break;
    }
    case AF_INET6: {
      while (nullptr != ai_ptr) {
        obj = new IPAddress;
        auto *addr = new struct sockaddr_in6;
        memcpy(addr, ai_ptr->ai_addr, ai_ptr->ai_addrlen);
        obj->p_->socket_address = reinterpret_cast<struct sockaddr *>(addr);
        list_ptr->deque_.push_back(obj->p_.get());
        ai_ptr = ai_ptr->ai_next;
      }
      break;
    }
    default: {
      fprintf(stderr, "%s\n", "Error! Unspecified family!");
      break;
    }
  }

  // Do not forget free addr_info:
  freeaddrinfo(addr_info);

  return list_ptr;
}

IPAddress::IPAddress() {
  p_ = std::make_unique<Private>(this);
}

IPAddress::IPAddress(const AddressInfo &address_info)
    : IPAddress() {
  switch (address_info.GetAddress()->sa_family) {
    case AF_INET: {
      auto *addr = new struct sockaddr_in;
      memcpy(addr, address_info.GetAddress(), address_info.GetAddressLength());
      p_->socket_address = reinterpret_cast<struct sockaddr *>(addr);
      break;
    }
    case AF_INET6: {
      auto *addr = new struct sockaddr_in6;
      memcpy(addr, address_info.GetAddress(), address_info.GetAddressLength());
      p_->socket_address = reinterpret_cast<struct sockaddr *>(addr);
      break;
    }
    default:break;
  }
}

IPAddress::IPAddress(const IPAddress &other)
    : IPAddress() {
  if (nullptr != other.p_->socket_address) {
    switch (other.p_->socket_address->sa_family) {
      case AF_INET: {
        auto *addr = new struct sockaddr_in;
        memcpy(addr,
               reinterpret_cast<struct sockaddr_in *>(other.p_->socket_address),
               sizeof(struct sockaddr_in));
        p_->socket_address = reinterpret_cast<struct sockaddr *>(addr);
        break;
      }
      case AF_INET6: {
        auto *addr = new struct sockaddr_in6;
        memcpy(addr,
               reinterpret_cast<struct sockaddr_in6 *>(other.p_->socket_address),
               sizeof(struct sockaddr_in6));
        p_->socket_address = reinterpret_cast<struct sockaddr *>(addr);
        break;
      }
      default:break;
    }
  }
}

IPAddress &IPAddress::operator=(const IPAddress &other) {
  Clear();

  if (nullptr != other.p_->socket_address) {
    switch (other.p_->socket_address->sa_family) {
      case AF_INET: {
        auto *addr = new struct sockaddr_in;
        memcpy(addr,
               reinterpret_cast<struct sockaddr_in *>(other.p_->socket_address),
               sizeof(struct sockaddr_in));
        p_->socket_address = reinterpret_cast<struct sockaddr *>(addr);
        break;
      }
      case AF_INET6: {
        auto *addr = new struct sockaddr_in6;
        memcpy(addr,
               reinterpret_cast<struct sockaddr_in6 *>(other.p_->socket_address),
               sizeof(struct sockaddr_in6));
        p_->socket_address = reinterpret_cast<struct sockaddr *>(addr);
        break;
      }
      default:break;
    }
  }

  return *this;
}

IPAddress::~IPAddress() {
  Clear();
}

AddressFamily IPAddress::GetAddressFamily() const {
  AddressFamily addr_family = kAddressFamilyUnspecified;
  if (nullptr != p_->socket_address) {
    switch (p_->socket_address->sa_family) {
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

  if (nullptr == p_->socket_address) return str;

  switch (p_->socket_address->sa_family) {
    case AF_INET: {
      auto *addr = reinterpret_cast<struct sockaddr_in *>(p_->socket_address);
      char buf[INET_ADDRSTRLEN];
      str = inet_ntop(AF_INET, &addr->sin_addr, buf, INET_ADDRSTRLEN);
      break;
    }
    case AF_INET6: {
      auto *addr = reinterpret_cast<struct sockaddr_in6 *>(p_->socket_address);
      char buf[INET6_ADDRSTRLEN];
      str = inet_ntop(AF_INET6, &addr->sin6_addr, buf, INET6_ADDRSTRLEN);
      break;
    }
    default: break;
  }

  return str;
}

bool IPAddress::ToHostAndService(std::string *host, std::string *service) {
  if (nullptr == p_->socket_address) return false;
  if (nullptr == host && nullptr == service) return false;

  char buf_host[NI_MAXHOST];
  char buf_service[NI_MAXSERV];

  socklen_t addrlen = sizeof(struct sockaddr_storage);

  if (0 == getnameinfo(p_->socket_address, addrlen, buf_host, NI_MAXHOST, buf_service, NI_MAXSERV, 0)) {
    if (nullptr != host) {
      host->assign(buf_host);
    }
    if (nullptr != service) {
      service->assign(buf_service);
    }
    return true;
  }

  return false;
}

IPAddress::NameInfo IPAddress::ToHostAndService() {
  NameInfo pair;

  if (nullptr == p_->socket_address) return std::pair<std::string, std::string>();

  char buf_host[NI_MAXHOST];
  char buf_service[NI_MAXSERV];

  socklen_t addrlen = sizeof(struct sockaddr_storage);

  if (0 == getnameinfo(p_->socket_address, addrlen, buf_host, NI_MAXHOST, buf_service, NI_MAXSERV, 0)) {
    pair.first.assign(buf_host);
    pair.second.assign(buf_service);
  }

  return pair;
}

void IPAddress::Clear() {
  if (nullptr == p_->socket_address) return;;

  switch (p_->socket_address->sa_family) {
    case AF_INET: {
      // IPv4
      auto *ipv4_address = reinterpret_cast<struct sockaddr_in *>(p_->socket_address);
      delete ipv4_address;
      break;
    }
    case AF_INET6: {
      // IPv6
      auto *ipv6_address = reinterpret_cast<struct sockaddr_in6 *>(p_->socket_address);
      delete ipv6_address;
      break;
    }
    default: {
      // TODO: check the address type
      delete p_->socket_address;
      break;
    }
  }

  p_->socket_address = nullptr;
}

// -------------

IPv4Address::IPv4Address() {
  auto *addr = new sockaddr_in;

  bzero(addr, sizeof(struct sockaddr_in));
  addr->sin_family = AF_INET;

  p_->socket_address = reinterpret_cast<struct sockaddr *>(addr);
}

IPv6Address::IPv6Address() {
  auto *addr = new sockaddr_in6;

  bzero(addr, sizeof(struct sockaddr_in6));
  addr->sin6_family = AF_INET6;

  p_->socket_address = reinterpret_cast<struct sockaddr *>(addr);
}

// -----------------

IPAddressList::~IPAddressList() {
  deque_.clear([](base::BinodeBase *obj) {
    auto *node = static_cast<IPAddress::Private *>(obj);
    IPAddress *addr = node->proprietor;
    delete addr;
  });
}

IPAddress *IPAddressList::operator[](int index) const {
  return deque_.at(index)->proprietor;
}

} // namespace net
} // namespace wiztk
