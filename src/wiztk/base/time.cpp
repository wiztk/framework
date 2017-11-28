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

#include "wiztk/base/time.hpp"

#include "numerical/clamp.hpp"

namespace wiztk {
namespace base {

Time::Time(int year, int month, int day, int hour, int minute, int second, int ms, int us, int ns) {
  using numerical::ClampT;

  struct tm t = {second, minute, hour, day, month - 1, year - 1900, 0, 0, 0};
  timespec_.tv_sec = mktime(&t);
  timespec_.tv_nsec = ClampT(ms, 0, 999) * 1000 * 1000 +
      ClampT(us, 0, 999) * 1000 +
      ClampT(ns, 0, 999);
}

Time Time::Now(Type type) {
  Time time;

  clockid_t clock = CLOCK_REALTIME;
  switch (type) {
    case kRealtime: break;
    case kRealtimeCoarse: {
      clock = CLOCK_REALTIME_COARSE;
      break;
    }
    case kMonotonic: {
      clock = CLOCK_MONOTONIC;
      break;
    }
    case kMonotonicCoarse: {
      clock = CLOCK_MONOTONIC_COARSE;
      break;
    }
    case kMonotonicRaw: {
      clock = CLOCK_MONOTONIC_RAW;
      break;
    }
    case kBoottime: {
      clock = CLOCK_BOOTTIME;
      break;
    }
    case kProcessCPUTimeID: {
      clock = CLOCK_PROCESS_CPUTIME_ID;
      break;
    }
    case kThreadCPUTimeID: {
      clock = CLOCK_THREAD_CPUTIME_ID;
      break;
    }
    default:break;
  }

  if (clock_gettime(clock, &time.timespec_))
    throw std::runtime_error("Error! Fail to get clock time!");

  return time;
}

struct timeval Time::ToTimeVal() const {
  timeval tv = {0, 0};
  tv.tv_sec = timespec_.tv_sec;
  tv.tv_usec = timespec_.tv_nsec / 1000;
  return tv;
}

time_t Time::ToTimeT() const {
  time_t t = timespec_.tv_sec;
  return t;
}

String Time::ToString() const {
  // TODO: support i18n localtime
  struct tm *t = localtime(&timespec_.tv_sec);
  return String(asctime(t));
}

Time Time::FromTimeVal(const struct timeval &tv) {
  Time time;
  time.timespec_.tv_sec = tv.tv_sec;
  time.timespec_.tv_nsec = tv.tv_usec * 1000;
  return time;
}

} // namespace base
} // namespace wiztk
