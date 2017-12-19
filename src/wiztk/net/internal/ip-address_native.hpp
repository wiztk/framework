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

#ifndef WIZTK_NET_INTERNAL_IP_ADDRESS_NATIVE_HPP_
#define WIZTK_NET_INTERNAL_IP_ADDRESS_NATIVE_HPP_

#include "wiztk/net/ip-address.hpp"

namespace wiztk {
namespace net {

class IPAddress::Native {

 public:

  static socklen_t GetLength(const IPAddress &address);

  static const struct sockaddr *GetSockAddr(const IPAddress &address) {
    return address.address_;
  }

};

}
}

#endif // WIZTK_NET_INTERNAL_IP_ADDRESS_NATIVE_HPP_