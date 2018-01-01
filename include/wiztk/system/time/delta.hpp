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

#ifndef WIZTK_SYSTEM_TIME_DELTA_HPP_
#define WIZTK_SYSTEM_TIME_DELTA_HPP_

#include "wiztk/system/time/clock.hpp"

#include <ctime>

namespace wiztk {
namespace system {
namespace time {

/**
 * @ingroup system_time
 * @brief Time delta of maximal nanosecond resolution
 */
class WIZTK_EXPORT Delta {

 public:

  explicit Delta(long sec = 0, long nsec = 0) {
    timespec_.tv_sec = sec;
    timespec_.tv_nsec = nsec;
  }

  Delta(const Clock &t1, const Clock &t2) {
    timespec_.tv_sec = t2.sec() - t1.sec();
    timespec_.tv_nsec = t2.nsec() - t1.nsec();
  }

  Delta(const Delta &other) = default;

  ~Delta() = default;

  Delta &operator=(const Delta &other) = default;

  Delta operator+(const Delta &other) const {
    return Delta(timespec_.tv_sec + other.timespec_.tv_sec,
                 timespec_.tv_nsec + other.timespec_.tv_nsec);
  }

  Delta operator-(const Delta &other) const {
    return Delta(timespec_.tv_sec - other.timespec_.tv_sec,
                 timespec_.tv_nsec - other.timespec_.tv_nsec);
  }

  long sec() const { return timespec_.tv_sec; }

  long nsec() const { return timespec_.tv_nsec; }

 private:

  struct timespec timespec_ = {0};

};

inline bool WIZTK_EXPORT operator==(const Delta &d1, const Delta &d2) {
  return d1.sec() == d2.sec() ? d1.nsec() == d2.nsec() : false;
}

inline bool WIZTK_EXPORT operator!=(const Delta &d1, const Delta &d2) {
  return d1.sec() != d2.sec() ? d1.nsec() != d2.nsec() : true;
}

inline bool WIZTK_EXPORT operator<(const Delta &d1, const Delta &d2) {
  return d1.sec() < d2.sec() ? d1.nsec() < d2.nsec() : false;
}

inline bool WIZTK_EXPORT operator>(const Delta &d1, const Delta &d2) {
  return d1.sec() > d2.sec() ? d1.nsec() > d2.nsec() : false;
}

inline bool WIZTK_EXPORT operator<=(const Delta &d1, const Delta &d2) {
  return d1.sec() <= d2.sec() ? d1.nsec() <= d2.nsec() : false;
}

inline bool WIZTK_EXPORT operator>=(const Delta &d1, const Delta &d2) {
  return d1.sec() >= d2.sec() ? d1.nsec() >= d2.nsec() : false;
}

} // namespace time
} // namespace system
} // namespace wiztk

#endif // WIZTK_SYSTEM_TIME_DELTA_HPP_
