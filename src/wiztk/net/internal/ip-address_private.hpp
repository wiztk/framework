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

#ifndef WIZTK_NET_INTERNAL_IP_ADDRESS_PRIVATE_HPP_
#define WIZTK_NET_INTERNAL_IP_ADDRESS_PRIVATE_HPP_

#include "wiztk/net/ip-address.hpp"

namespace wiztk {
namespace net {

using base::CountedDequeNode;

/**
 * @ingroup net_intern
 * @brief Private data in IPAddress.
 */
struct WIZTK_NO_EXPORT IPAddress::Private : public CountedDequeNode<IPAddress::Private> {

  /**
   * @brief Default constructor is disabled
   */
  Private() = delete;

  /**
   * @brief Constructor with a given IPAddress object as proprietor.
   * @param address
   */
  explicit Private(IPAddress *address)
      : proprietor(address) {}

  /**
   * @brief Copy constructor.
   * @param other
   */
  Private(const Private &other);

  /**
   * @brief Destructor.
   *
   * This will release the memory allocated for socket_address.
   */
  ~Private() final;

  /**
   * @brief Copy assignment.
   * @param other
   * @return
   */
  Private &operator=(const Private &other);

  /**
   * @brief Release the memory allocated for socket_address.
   */
  void Reset();

  void Copy(const Private &other);

  IPAddress *proprietor = nullptr;

  struct sockaddr *socket_address = nullptr;

};

} // namespace net
} // namespace wiztk

#endif // WIZTK_NET_INTERNAL_IP_ADDRESS_PRIVATE_HPP_
