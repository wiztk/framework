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

#ifndef WIZTK_NET_ADDRESS_INFO_HPP_
#define WIZTK_NET_ADDRESS_INFO_HPP_

#include "wiztk/base/deque.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <memory>

namespace wiztk {
namespace net {

// Forward declaration:
class AddressInfoList;

/**
 * @ingroup net
 * @brief Represents address and hostname.
 *
 * It's unable to create a single AddressInfo object by constructor. An AddressInfo
 * object is always a node in AddressInfoList created by GetAll().
 */
class AddressInfo : protected base::BinodeBase {

  template<typename T> friend
  class base::Deque;

  friend class AddressInfoList;
  friend class IPAddress;

 public:

  /**
   * @brief Creates an AddressInfo list by given host, service and hints.
   * @param host
   * @param service
   * @param hints
   * @return
   */
  static std::unique_ptr<AddressInfoList> GetAll(const char *host = nullptr,
                                                 const char *service = nullptr,
                                                 const AddressInfo *hints = nullptr);

  /**
   * @brief Destructor.
   */
  ~AddressInfo() final;

  int flags() const { return address_info_->ai_flags; }

  /**
   * @brief Returns address family.
   * @return
   *
   * Valid values include AF_INET and AF_INET6.
   */
  int family() const { return address_info_->ai_family; }

  int socket_type() const { return address_info_->ai_socktype; }

  int protocol() const { return address_info_->ai_protocol; }

  socklen_t addrlen() const { return address_info_->ai_addrlen; }

  struct sockaddr *addr() const { return address_info_->ai_addr; }

  const char *canonical_name() const { return address_info_->ai_canonname; }

 private:

  AddressInfo() = default;

  struct addrinfo *address_info_ = nullptr;

};

/**
 * @ingroup net
 * @brief A list of AddressInfo related to the same host and service.
 */
class AddressInfoList {

  friend class AddressInfo;

 public:

  ~AddressInfoList();

  AddressInfo *at(int index) const { return deque_.GetAt(index); }

  AddressInfo *operator[](int index) const { return deque_.GetAt(index); }

  size_t size() const { return deque_.GetCount(); }

 private:

  AddressInfoList() = default;

  // We use has-a relationship here.
  base::Deque<AddressInfo> deque_;

};

} // namespace net
} // namespace wiztk

#endif // WIZTK_NET_ADDRESS_INFO_HPP_
