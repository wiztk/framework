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

#ifndef WIZTK_ASYNC_EVENT_LOOP_PRIVATE_HPP_
#define WIZTK_ASYNC_EVENT_LOOP_PRIVATE_HPP_

#include "wiztk/async/event-loop.hpp"

#include "wiztk/system/threading/thread-local.hpp"

namespace wiztk {
namespace async {

/**
 * @brief Private structure in EventLoop.
 */
struct EventLoop::Private {

  /**
   * @brief A static object stores EventLoop in each thread.
   */
  static system::threading::ThreadLocal<EventLoop> kPerThreadStorage;

};

}
}

#endif // WIZTK_ASYNC_EVENT_LOOP_PRIVATE_HPP_
