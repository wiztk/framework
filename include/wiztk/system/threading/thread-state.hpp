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

#ifndef WIZTK_SYSTEM_THREADING_THREAD_STATE_HPP_
#define WIZTK_SYSTEM_THREADING_THREAD_STATE_HPP_

namespace wiztk {
namespace system {
namespace threading {

/**
 * @brief Enumeration indicates the state of a Thread.
 */
enum ThreadState {

  kNew, /**< A thread that has not started */

  kRunning, /**< The thread has been started */

  kTerminated,  /**< A thread that has exited */

};

}
}
}

#endif // WIZTK_SYSTEM_THREADING_THREAD_STATE_HPP_
