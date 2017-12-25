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

#include "internal/address-info_private.hpp"

namespace wiztk {
namespace net {

std::unique_ptr<AddressInfoList> AddressInfo::GetAll(const char *host,
                                                     const char *service,
                                                     const AddressInfo *hints) {
  std::unique_ptr<AddressInfoList> list_ptr(new AddressInfoList);

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

    throw std::runtime_error("Error! Cannot get address info!");
  }

  struct addrinfo *ai_ptr = addr_info;
  AddressInfo *tmp = nullptr;
  while (nullptr != ai_ptr) {
    tmp = new AddressInfo;
    tmp->p_->address_info = ai_ptr;
    list_ptr->deque_.push_back(tmp->p_.get());
    ai_ptr = ai_ptr->ai_next;
  }

  return list_ptr;
}

AddressInfo::AddressInfo() {
  p_ = std::make_unique<Private>(this);
}

AddressInfo::~AddressInfo() {
  if (nullptr != p_->address_info) {
    freeaddrinfo(p_->address_info);
  }
}

int AddressInfo::GetFlags() const {
  return p_->address_info->ai_flags;
}

int AddressInfo::GetFamily() const {
  return p_->address_info->ai_family;
}

int AddressInfo::GetSocketType() const {
  return p_->address_info->ai_socktype;
}

int AddressInfo::GetProtocol() const {
  return p_->address_info->ai_protocol;
}

socklen_t AddressInfo::GetAddressLength() const {
  return p_->address_info->ai_addrlen;
}

struct sockaddr *AddressInfo::GetAddress() const {
  return p_->address_info->ai_addr;
}

const char *AddressInfo::GetCanonicalName() const {
  return p_->address_info->ai_canonname;
}

// --------------------------------------

AddressInfoList::~AddressInfoList() {
  if (deque_.count() > 0) {
    freeaddrinfo(deque_[0]->address_info);
  }

  for (AddressInfoDeque::Iterator it = deque_.begin(); it; ++it) {
    it.get()->address_info = nullptr;
  }

  Clear();
}

AddressInfo *AddressInfoList::operator[](int index) const {
  return deque_.at(index)->proprietor;
}

void AddressInfoList::Clear() {
  deque_.clear([](base::BinodeBase *obj) {
    auto *node = static_cast<AddressInfo::Private *>(obj);
    AddressInfo *addr_info = node->proprietor;
    delete addr_info;
  });
}

} // namespace net
} // namespace wiztk
