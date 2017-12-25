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

#ifndef WIZTK_SYSTEM_TIME_HPP_
#define WIZTK_SYSTEM_TIME_HPP_

#include "wiztk/base/macros.hpp"
#include "wiztk/base/string.hpp"

#include <ctime>

namespace wiztk {
namespace system {

/**
 * @ingroup base
 * @brief Time class with nanosecond resolution
 */
class WIZTK_EXPORT Time {

 public:

  using String = base::String;

  class Delta;

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
  explicit Time(const struct timespec &ts)
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
   * @brief Default copy constructor
   * @param other
   */
  Time(const Time &other) = default;

  /**
   * @brief Default move constructor
   * @param other
   */
  Time(Time &&other) = default;

  /**
   * @brief Default destructor
   */
  ~Time() = default;

  /**
   * @brief Default assignment
   * @param other
   * @return
   */
  Time &operator=(const Time &other) = default;

  Time &operator=(Time &&other) = default;

  struct timeval ToTimeVal() const;

  time_t ToTimeT() const;

  String ToString() const;

  Time Approximate(Precision prec) const;

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

  /**
   * @brief Returns the current time.
   * @param type
   * @return
   */
  static Time Now(Precision precision = kMilliseconds);

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

/**
 * @ingroup base
 * @brief Time delta of maximal nanosecond resolution
 */
class WIZTK_EXPORT Time::Delta {

 public:

  explicit Delta(long sec = 0, long nsec = 0) {
    delta_.tv_sec = sec;
    delta_.tv_nsec = nsec;
  }

  Delta(const Time &t1, const Time &t2) {
    delta_.tv_sec = t2.sec() - t1.sec();
    delta_.tv_nsec = t2.nsec() - t1.nsec();
  }

  Delta(const Delta &other) = default;

  ~Delta() = default;

  Delta &operator=(const Delta &other) = default;

  Delta operator+(const Delta &other) const {
    Delta d;
    d.delta_.tv_sec = delta_.tv_sec + other.delta_.tv_sec;
    d.delta_.tv_nsec = delta_.tv_nsec + other.delta_.tv_nsec;
    return d;
  }

  Delta operator-(const Delta &other) const {
    Delta d;
    d.delta_.tv_sec = delta_.tv_sec - other.delta_.tv_sec;
    d.delta_.tv_nsec = delta_.tv_nsec - other.delta_.tv_nsec;
    return d;
  }

  Delta Approximate(Precision prec) const;

  long sec() const { return delta_.tv_sec; }

  long nsec() const { return delta_.tv_nsec; }

 private:

  struct timespec delta_;

};

WIZTK_EXPORT inline bool operator==(const Time::Delta &d1, const Time::Delta &d2) {
  return d1.sec() == d2.sec() ? d1.nsec() == d2.nsec() : false;
}

WIZTK_EXPORT inline bool operator!=(const Time::Delta &d1, const Time::Delta &d2) {
  return d1.sec() != d2.sec() ? d1.nsec() != d2.nsec() : true;
}

WIZTK_EXPORT inline bool operator<(const Time::Delta &d1, const Time::Delta &d2) {
  return d1.sec() < d2.sec() ? d1.nsec() < d2.nsec() : false;
}

WIZTK_EXPORT inline bool operator>(const Time::Delta &d1, const Time::Delta &d2) {
  return d1.sec() > d2.sec() ? d1.nsec() > d2.nsec() : false;
}

WIZTK_EXPORT inline bool operator<=(const Time::Delta &d1, const Time::Delta &d2) {
  return d1.sec() <= d2.sec() ? d1.nsec() <= d2.nsec() : false;
}

WIZTK_EXPORT inline bool operator>=(const Time::Delta &d1, const Time::Delta &d2) {
  return d1.sec() >= d2.sec() ? d1.nsec() >= d2.nsec() : false;
}

} // namespace system
} // namespace wiztk

#endif // WIZTK_BASE_TIME_HPP_
