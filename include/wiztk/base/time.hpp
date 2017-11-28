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

#ifndef WIZTK_BASE_TIME_HPP_
#define WIZTK_BASE_TIME_HPP_

#include "macros.hpp"
#include "string.hpp"

#include <ctime>

namespace wiztk {
namespace base {

/**
 * @ingroup base
 * @brief Time class with nanosecond resolution
 */
class WIZTK_EXPORT Time {

 public:

  enum Type {
    kRealtime,
    kRealtimeCoarse,
    kMonotonic,
    kMonotonicCoarse,
    kMonotonicRaw,
    kBoottime,
    kProcessCPUTimeID,
    kThreadCPUTimeID
  };

  enum Resolution {
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

  /**
   * @brief Default constructor
   */
  Time() {
    timespec_.tv_sec = 0;
    timespec_.tv_nsec = 0;
  }

  /**
   * @brief Constructor from a timespec value
   * @param ts
   */
  Time(const struct timespec &ts)
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
  Time(int year,
       int month,
       int day,
       int hour,
       int minute,
       int second = 0,
       int ms = 0,
       int us = 0,
       int ns = 0);

  /**
   * @brief Copy constructor
   * @param other
   */
  Time(const Time &other)
      : timespec_(other.timespec_) {}

  /**
   * @brief Default move constructor
   * @param other
   */
  Time(Time &&other) = default;

  /**
   * @brief Default destructor
   */
  ~Time() = default;

  Time &operator=(const Time &other) {
    timespec_ = other.timespec_;
    return *this;
  }

  Time &operator=(Time &&other) = default;

  struct timeval ToTimeVal() const;

  time_t ToTimeT() const;

  String ToString() const;

  long sec() const { return timespec_.tv_sec; }

  long nsec() const { return timespec_.tv_nsec; }

  /**
   * @brief Returns the current time.
   * @param type
   * @return
   */
  static Time Now(Type type = kRealtime);

  static Time FromTimeVal(const struct timeval &tv);

 private:

  struct timespec timespec_;

};

WIZTK_EXPORT inline bool operator==(const Time &t1, const Time &t2) {
  return (t1.sec() == t2.sec()) && (t1.nsec() == t2.nsec());
}

WIZTK_EXPORT inline bool operator!=(const Time &t1, const Time &t2) {
  return (t1.sec() != t2.sec()) || (t1.nsec() != t2.nsec());
}

WIZTK_EXPORT inline bool operator<(const Time &t1, const Time &t2) {
  return t1.sec() < t2.sec() ? true : (t1.nsec() < t2.nsec());
}

WIZTK_EXPORT inline bool operator>(const Time &t1, const Time &t2) {
  return t1.sec() > t2.sec() ? true : (t1.nsec() > t2.nsec());
}

WIZTK_EXPORT inline Time operator+(const Time &t1, const Time &t2) {
  struct timespec ts{t1.sec() + t2.sec(), t1.nsec() + t2.nsec()};
  return Time(ts);
}

} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_TIME_HPP_
