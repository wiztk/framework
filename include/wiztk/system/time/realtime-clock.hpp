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

#ifndef WIZTK_SYSTEM_TIME_REALTIME_CLOCK_HPP_
#define WIZTK_SYSTEM_TIME_REALTIME_CLOCK_HPP_

#include "wiztk/system/time/clock.hpp"

namespace wiztk {
namespace system {
namespace time {

/**
 * @ingroup system_time
 * @brief System-wide clock that measures real (i.e.,  wall-clock)  time.
 */
class WIZTK_EXPORT RealtimeClock : public Clock {

 public:

  using Clock::Clock;

  ~RealtimeClock() final = default;

  static RealtimeClock Now(Precision precision = kMilliseconds);

};

} // namespace time
} // namespace system
} // namespace wiztk

#endif // WIZTK_SYSTEM_TIME_REALTIME_CLOCK_HPP_
