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

#ifndef WIZTK_NET_ADDRESS_FAMILY_HPP_
#define WIZTK_NET_ADDRESS_FAMILY_HPP_

#include <sys/socket.h>

namespace wiztk {
namespace net {

/**
 * @ingroup net
 * @brief Address family.
 */
enum AddressFamily {
  kAddressFamilyUnspecified = AF_UNSPEC,  /**< Unspecified */
  kAddressFamilyUNIX = AF_UNIX,    /**< Local communication */
  kAddressFamilyLocal = AF_LOCAL,   /**< Local communication */
  kAddressFamilyINET = AF_INET,    /**< AF_INET - IPv4 Internet protocols */
  kAddressFamilyINET6 = AF_INET6,   /**< AF_INET6 - IPv6 Internet protocols */
  kAddressFamilyIPX = AF_IPX,     /**< AF_IPX - Novell protocols */
  kAddressFamilyNetLink = AF_NETLINK, /**< AF_NETLINK - Kernel user interface device */
  kAddressFamilyX25 = AF_X25,     /**< AF_X25 - ITU-T X.25 / ISO-8208 protocol */
  kAddressFamilyAX25 = AF_AX25,    /**< AF_AX25 - Amateur radio AX.25 protocol */
  kAddressFamilyATMPVC = AF_ATMPVC,  /**< AF_ATMPVC - Access to raw ATM PVCs */
  kAddressFamilyAppleTalk = AF_APPLETALK, /**< AF_APPLETALK - AppleTalk */
  kAddressFamilyPacket = AF_PACKET,  /**< AF_PACKET - Low level packet interface */
  kAddressFamilyLast                    /**< LAST */
};

}  // namespace net
}  // namespace wiztk

#endif  // WIZTK_ADDRESS_FAMILY_HPP_
