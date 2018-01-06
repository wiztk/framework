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

#include "wiztk/base/counted-deque.hpp"

#include "wiztk/net/address-family.hpp"

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <memory>

namespace wiztk {
namespace net {

// Forward declaration:
class AddressInfo;
class IPAddressList;

/**
 * @ingroup net
 * @brief This class represents an Internet Protocol (IP) address.
 *
 * An IP address is either a 32-bit (IPV4) or 128-bit (IPV6) unsigned number
 * used by IP, a lower-level protocol on which protocols like TCP and UDP are
 * built.
 */
class WIZTK_EXPORT IPAddress {

  friend class IPAddressList;

 public:

  class Native;

  /**
   * @brief A typedef of host/service pair.
   */
  typedef std::pair<std::string, std::string> NameInfo;

  /**
   * @brief Get an IPAddressList by given address info.
   * @param host
   * @param service
   * @param hints
   * @return
   */
  static std::unique_ptr<IPAddressList> GetByHostAndService(const char *host,
                                                            const char *service = nullptr,
                                                            const AddressInfo *hints = nullptr);

  /**
   * @brief Constructor by an AddressInfo.
   * @param address_info
   */
  explicit IPAddress(const AddressInfo &address_info);

  /**
   * @brief Copy constructor.
   * @param other
   */
  IPAddress(const IPAddress &other);

  IPAddress(IPAddress &&other) noexcept = default;

  IPAddress &operator=(const IPAddress &other);

  IPAddress &operator=(IPAddress &&other) noexcept = default;

  virtual ~IPAddress();

  AddressFamily GetAddressFamily() const;

  std::string ToString() const;

  bool ToHostAndService(std::string *host, std::string *service);

  NameInfo ToHostAndService();

 protected:

  IPAddress();

  struct Private;

  std::unique_ptr<Private> p_;

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

/**
 * @ingroup net
 * @brief A list of IPAddress.
 */
class IPAddressList {

  friend class IPAddress;

 public:

  /**
   * @brief Destructor.
   *
   * Clean and destroy all IPAddress objects.
   */
  ~IPAddressList();

  /**
   * @brief Get the IPAddress object at the given index.
   * @param index
   * @return
   */
  IPAddress *at(int index) const {
    return operator[](index);
  }

  /**
   * @brief Get the IPAddress object at the given index.
   * @param index
   * @return
   */
  IPAddress *operator[](int index) const;

  /**
   * @brief Get the size of this list.
   * @return
   */
  size_t size() const { return deque_.count(); }

 private:

  typedef base::CountedDeque<IPAddress::Private> IPAddressDeque;

  /**
   * @brief Default constructor.
   */
  IPAddressList() = default;

  IPAddressDeque deque_;

};

} // namespace net
} // namespace wiztk

#endif // WIZTK_NET_INET_ADDRESS_HPP_
