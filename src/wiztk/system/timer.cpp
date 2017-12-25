/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
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

#include "wiztk/system/timer.hpp"

#include <cerrno>

namespace wiztk {
namespace system {

Timer::Timer()
    : id_(nullptr),
      interval_(0),
      is_armed_(false) {
  id_ = Create();
}

Timer::~Timer() {
  if (nullptr != id_) {
    if (0 != timer_delete(id_)) {
      _DEBUG("%s\n", "Not a valid POSIX timer id");
    }
  }
}

void Timer::Start() {
  if (nullptr == id_) return;
  is_armed_ = SetTime();
}

void Timer::Stop() {
  if (!is_armed_) return;

  int ret = 0;
  struct itimerspec ts = {0};

  ret = timer_settime(id_, 0, &ts, nullptr);
  if (ret < 0) {
    _DEBUG("%s\n", "Fail to stop timer");
  }

  is_armed_ = false;
}

void Timer::SetInterval(unsigned int interval) {
  if ((nullptr == id_) || (interval_ == interval)) return;

  interval_ = interval;

  if (is_armed_) is_armed_ = SetTime();
}

timer_t Timer::Create() {
  struct sigevent sev = {0};
  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_value.sival_ptr = this;
  sev.sigev_notify_function = OnExpire;
  sev.sigev_notify_attributes = nullptr;

  timer_t timer = nullptr;

  int ret = timer_create(CLOCK_REALTIME, &sev, &timer);
  if (ret < 0) {
    _DEBUG("%s\n", "Fail to create timer");
    if (ret == EAGAIN) {
      _DEBUG("%s\n", "Temporary error during kernel allocation of timer structures.");
    } else if (ret == EINVAL) {
      _DEBUG("%s\n", "Clock ID, sigev_notify, sigev_signo, or sigev_notify_thread_id is invalid.");
    } else if (ret == ENOMEM) {
      _DEBUG("%s\n", "Could not allocate memory.");
    }
    return nullptr;
  }

  return timer;
}

bool Timer::SetTime() {
  int ret = 0;
  struct itimerspec ts = {0};
  unsigned int sec = interval_ / 1000;
  long nsec = (interval_ % 1000) * 1000 * 1000;

  ts.it_interval.tv_sec = sec;
  ts.it_interval.tv_nsec = nsec;
  ts.it_value.tv_sec = sec;
  ts.it_value.tv_nsec = nsec;

  ret = timer_settime(id_, 0, &ts, 0);
  if (ret < 0) {
    _DEBUG("%s\n", "Fail to set timer");
    return false;
  } else {
    return true;
  }
}

void Timer::OnExpire(union sigval sigev_value) {
  auto *_this = static_cast<Timer *>(sigev_value.sival_ptr);
  if (_this->expire_) _this->expire_.Invoke();
}

} // namespace base
} // namespace wiztk
