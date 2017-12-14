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

#ifndef WIZTK_NET_SOCKET_TYPE_HPP_
#define WIZTK_NET_SOCKET_TYPE_HPP_

#include <sys/types.h>
#include <sys/socket.h>

namespace wiztk {
namespace net {

/**
 * @ingroup net
 * @brief Socket type.
 */
enum SocketType {

  kSocketStream,                        //< SOCK_STREAM
  kSocketDatagram,                      //< SOCK_DGRAM
  kSocketSequencedPacket,               //< SOCK_SEQPACKET
  kSocketRaw,                           //< SOCK_RAW
  kSocketReliableDatagram,              //< SOCK_RDM
  kSocketPacket,                        //< SOCK_PACKET
  kSocketLast                           //< Last
  
};

}  // namespace net
}  // namespace wiztk

#endif  // WIZTK_NET_SOCKET_TYPE_HPP_
