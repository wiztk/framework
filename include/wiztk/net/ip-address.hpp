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

#ifndef WIZTK_NET_IP_ADDRESS_HPP_
#define WIZTK_NET_IP_ADDRESS_HPP_

#include "wiztk/base/deque.hpp"

#include "wiztk/net/address-family.hpp"

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <memory>

namespace wiztk {
namespace net {

// Forward declaration:
class IPAddressList;

/**
 * @ingroup net
 * @brief This class represents an Internet Protocol (IP) address.
 *
 * An IP address is either a 32-bit (IPV4) or 128-bit (IPV6) unsigned number
 * used by IP, a lower-level protocol on which protocols like TCP and UDP are
 * built.
 */
class IPAddress : protected base::BiNode {

  template<typename T> friend
  class base::Deque;

 public:

  static std::unique_ptr<IPAddressList> GetByName(const char *name);

  IPAddress(IPAddress &&other) noexcept = default;

  IPAddress &operator=(IPAddress &&other)noexcept = default;

  ~IPAddress() override;

  AddressFamily GetAddressFamily() const;

  std::string ToString() const;

 protected:

  IPAddress() = default;

  struct sockaddr *address_ = nullptr;

};

/**
 * @ingroup net
 * @brief IPv4 address
 */
class IPv4Address : public IPAddress {

 public:

  IPv4Address();

  ~IPv4Address() final = default;

};

/**
 * @ingroup net
 * @brief IPv6 address
 */
class IPv6Address : public IPAddress {

 public:

  IPv6Address();

  ~IPv6Address() final = default;

};

class IPAddressList {

  friend class IPAddress;

 public:

  IPAddressList() = default;

  ~IPAddressList() = default;

  IPAddress *GetAt(int index) const { return deque_.GetAt(index); }

  IPAddress *operator[](int index) const { return deque_.GetAt(index); }

  size_t size() const { return deque_.GetSize(); }

 private:

  class Deque : public base::Deque<IPAddress> {};

  Deque deque_;

};

} // namespace net
} // namespace wiztk

#endif // WIZTK_NET_INET_ADDRESS_HPP_
