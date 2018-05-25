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

#ifndef WIZTK_NET_INTERNAL_ADDRESS_INFO_PRIVATE_HPP_
#define WIZTK_NET_INTERNAL_ADDRESS_INFO_PRIVATE_HPP_

#include "wiztk/net/address-info.hpp"

namespace wiztk {
namespace net {

using base::CountedDequeNode;

struct WIZTK_NO_EXPORT AddressInfo::Private : public CountedDequeNode<AddressInfo::Private> {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);
  Private() = delete;

  explicit Private(AddressInfo *addr_info)
      : proprietor(addr_info) {}

  AddressInfo *proprietor = nullptr;

  struct addrinfo *address_info = nullptr;

};

} // namespace net
} // namespace wiztk

#endif // WIZTK_NET_INTERNAL_ADDRESS_INFO_PRIVATE_HPP_
