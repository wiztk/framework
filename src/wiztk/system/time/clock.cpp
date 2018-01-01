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

#include "wiztk/system/time/clock.hpp"

#include "wiztk/numerical/clamp.hpp"

namespace wiztk {
namespace system {
namespace time {

Clock::Clock(int year, int month, int day, int hour, int minute, int second, int ms, int us, int ns) {
  using numerical::Clamp;

  struct tm t = {second, minute, hour, day, month - 1, year - 1900, 0, 0, 0};
  timespec_.tv_sec = mktime(&t);
  timespec_.tv_nsec = Clamp(ms, 0, 999) * 1000 * 1000 +
      Clamp(us, 0, 999) * 1000 +
      Clamp(ns, 0, 999);
}

struct timeval Clock::ToTimeVal() const {
  timeval tv = {0, 0};
  tv.tv_sec = timespec_.tv_sec;
  tv.tv_usec = timespec_.tv_nsec / 1000;
  return tv;
}

std::string Clock::ToString() const {
  // TODO: support i18n localtime
  struct tm *t = localtime(&timespec_.tv_sec);
  return asctime(t);
}

}
}
}