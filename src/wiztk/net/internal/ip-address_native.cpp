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

#include "ip-address_native.hpp"

namespace wiztk {
namespace net {

socklen_t IPAddress::Native::GetLength(const IPAddress &address) {
  socklen_t length = 0;

  switch (address.address_->sa_family) {
    case AF_INET: {
      length = sizeof(struct sockaddr_in);
      break;
    }
    case AF_INET6: {
      length = sizeof(struct sockaddr_in6);
      break;
    }
    default:break;
  }

  return length;
}

}
}