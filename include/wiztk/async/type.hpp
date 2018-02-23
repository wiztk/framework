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

#ifndef WIZTK_ASYNC_TYPE_HPP_
#define WIZTK_ASYNC_TYPE_HPP_

#include <sys/epoll.h>

namespace wiztk {
namespace async {

enum EventType {

  kRead = EPOLLIN,  /**< Data other than high-priority data can be read */

  kPriority = EPOLLPRI, /**< High-priority data can be read */

  kWrite = EPOLLOUT,  /**< Normal data can be written */

  kShutdown = EPOLLRDHUP, /**< Shutdown on peer socket */

  kEdgeTriggered = EPOLLET, /**< Employ edge-triggered event notification */

  kError = EPOLLERR,  /**< An error has occurred */

  kHangup = EPOLLHUP  /**< A hangup has occurred */

};

} // namespace async
} // namespace wiztk

#endif // WIZTK_ASYNC_TYPE_HPP_
