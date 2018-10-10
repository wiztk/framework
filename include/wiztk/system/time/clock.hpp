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

#ifndef WIZTK_SYSTEM_TIME_CLOCK_HPP_
#define WIZTK_SYSTEM_TIME_CLOCK_HPP_

#include "wiztk/base/macros.hpp"

#include <ctime>
#include <string>

namespace wiztk {
namespace system {
namespace time {

/**
 * @ingroup system_time
 * @brief Class represents a POSIX clock.
 */
class WIZTK_EXPORT Clock {

 public:

  enum Precision {
    kYears,
    kMonths,
    kWeeks,
    kDays,
    kHours,
    kMinutes,
    kSeconds,
    kMilliseconds,
    kMicroseconds,
    kNanoseconds
  };

  Clock() = default;

  explicit Clock(const struct timespec &ts)
      : timespec_(ts) {}

  /**
   * @brief Constructor with date time
   * @param year Year
   * @param month Month (1 - 12)
   * @param day Day of the month (1 - 31)
   * @param hour Hours (0 - 23)
   * @param minute Minutes (0 - 59)
   * @param second Seconds (0 - 60), normally in the range 0 to 59, but can be up to 60 to allow for leap seconds.
   * @param ms Milliseconds (0 - 999)
   * @param us Microseconds (0 - 999)
   * @param ns Nanoseconds (0 - 999)
   */
  Clock(int year,
        int month,
        int day,
        int hour,
        int minute,
        int second = 0,
        int ms = 0,
        int us = 0,
        int ns = 0);

  Clock(const Clock &other) = default;

  Clock(Clock &&other) = default;

  virtual ~Clock() = default;

  Clock &operator=(const Clock &other) = default;

  Clock &operator=(Clock &&other) = default;

  struct timeval ToTimeVal() const;

  std::string ToString() const;

  /**
   * @brief Get the seconds part of the time value
   * @return
   */
  long sec() const { return timespec_.tv_sec; }

  /**
   * @brief Get the nanoseconds part of the time value
   * @return
   */
  long nsec() const { return timespec_.tv_nsec; }

  const timespec *data() const { return &timespec_; }

 protected:

  struct timespec timespec_ = {0};

};

inline bool WIZTK_EXPORT operator==(const Clock &c1, const Clock &c2) {
  return (c1.sec() == c2.sec()) && (c1.nsec() == c2.nsec());
}

inline bool WIZTK_EXPORT operator!=(const Clock &t1, const Clock &t2) {
  return (t1.sec() != t2.sec()) || (t1.nsec() != t2.nsec());
}

inline bool WIZTK_EXPORT  operator<(const Clock &t1, const Clock &t2) {
  return t1.sec() < t2.sec() ? true : (t1.nsec() < t2.nsec());
}

inline bool WIZTK_EXPORT operator>(const Clock &t1, const Clock &t2) {
  return t1.sec() > t2.sec() ? true : (t1.nsec() > t2.nsec());
}

inline Clock WIZTK_EXPORT operator+(const Clock &t1, const Clock &t2) {
  struct timespec ts{t1.sec() + t2.sec(), t1.nsec() + t2.nsec()};
  return Clock(ts);
}

} // namespace time
} // namespace system
} // namespace wiztk

#endif // WIZTK_SYSTEM_TIME_CLOCK_HPP_
