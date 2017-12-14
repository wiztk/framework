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

#include "wiztk/net/address-info.hpp"

namespace wiztk {
namespace net {

std::unique_ptr<AddressInfoList> AddressInfo::GetAll(const char *host,
                                                     const char *service,
                                                     const AddressInfo *hints) {
  std::unique_ptr<AddressInfoList> list_ptr(new AddressInfoList);

  struct addrinfo *native = nullptr;

  int result = getaddrinfo(host,
                           service,
                           nullptr == hints ? nullptr : hints->address_info_,
                           &native);

  if (result) {
    if (nullptr != native) {
      freeaddrinfo(native);
    }

    // TODO: check the return value

    throw std::runtime_error("Error! Cannot get address info!");
  }

  struct addrinfo *ai_ptr = native;
  AddressInfo *tmp = nullptr;
  while (nullptr != ai_ptr) {
    tmp = new AddressInfo;
    tmp->address_info_ = ai_ptr;
    list_ptr->deque_.PushBack(tmp);
    ai_ptr = ai_ptr->ai_next;
  }

  return list_ptr;
}

AddressInfo::~AddressInfo() {
  if (nullptr != address_info_) {
    freeaddrinfo(address_info_);
  }
}

// --------------------------------------

AddressInfoList::~AddressInfoList() {
  if (deque_.GetSize() > 0) {
    freeaddrinfo(deque_[0]->address_info_);
  }

  for (Deque::Iterator it = deque_.begin(); it; ++it) {
    it.element()->address_info_ = nullptr;
  }
}

} // namespace net
} // namespace wiztk
