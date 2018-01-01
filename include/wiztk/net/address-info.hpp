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

#include "wiztk/base/counted-deque.hpp"

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
class WIZTK_EXPORT AddressInfo {

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
  ~AddressInfo();

  int GetFlags() const;

  /**
   * @brief Returns address family.
   * @return
   *
   * Valid values include AF_INET and AF_INET6.
   */
  int GetFamily() const;

  int GetSocketType() const;

  int GetProtocol() const;

  socklen_t GetAddressLength() const;

  struct sockaddr *GetAddress() const;

  const char *GetCanonicalName() const;

 private:

  // Forward declaration:
  struct Private;

  /**
   * @brief Default constructor.
   */
  AddressInfo();

  std::unique_ptr<Private> p_;

};

/**
 * @ingroup net
 * @brief A list of AddressInfo related to the same host and service.
 */
class WIZTK_EXPORT AddressInfoList {

  friend class AddressInfo;

 public:

  /**
   * @brief Destructor.
   *
   * Clean and destroy all AddressInfo objects.
   */
  ~AddressInfoList();

  /**
   * @brief Get the AddressInfo object at the given index.
   * @param index
   * @return
   */
  AddressInfo *at(int index) const {
    return operator[](index);
  }

  /**
   * @brief Get the AddressInfo object at the given index.
   * @param index
   * @return
   */
  AddressInfo *operator[](int index) const;

  /**
   * @brief Get the size of this list.
   * @return
   */
  size_t size() const { return deque_.count(); }

 private:

  typedef base::CountedDeque<AddressInfo::Private> AddressInfoDeque;

  AddressInfoList() = default;

  void Clear();

  // We use has-a relationship here.
  AddressInfoDeque deque_;

};

} // namespace net
} // namespace wiztk

#endif // WIZTK_NET_ADDRESS_INFO_HPP_
