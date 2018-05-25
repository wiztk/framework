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

#include <wiztk/gui/timer.hpp>

#include <wiztk/gui/abstract-epoll-task.hpp>
#include <wiztk/gui/application.hpp>

#include <sys/timerfd.h>
#include <unistd.h>
#include <sys/epoll.h>

#include "wiztk/base/macros.hpp"

namespace wiztk {
namespace gui {

class Timer::EpollTask : public AbstractEpollTask {

 public:

  EpollTask(Timer *timer)
      : AbstractEpollTask(), timer_(timer) {}

  virtual ~EpollTask() {}

  virtual void Run(uint32_t events) override;

 private:

  Timer *timer_;

};

struct Timer::Private {

  Private() = delete;
  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private(Timer *timer)
      : fd(-1), is_armed(false), interval(0), epoll_task(timer) {}

  ~Private() {}

  int fd;
  bool is_armed;
  unsigned int interval;  // interval in microseconds
  EpollTask epoll_task;

};

void Timer::EpollTask::Run(uint32_t events) {
  uint64_t exp;
  ssize_t s;

  s = read(timer_->p_->fd, &exp, sizeof(uint64_t));
  if (s != sizeof(uint64_t)) {
    _DEBUG("%s\n", "Fail to read timer fd!\n");
    return;
  }

  timer_->timeout_.Emit();
}

Timer::Timer(unsigned int interval) {
  p_.reset(new Private(this));

  p_->fd = timerfd_create(CLOCK_REALTIME, 0);
  if (p_->fd < 0) {
    fprintf(stderr, "Error! Fail to create timerfd!\n");
  }
  p_->interval = interval;
}

Timer::~Timer() {
  if (p_->fd >= 0) close(p_->fd);
}

void Timer::Start() {
  if (p_->is_armed) return;

//  Application::GetInstance()->WatchFileDescriptor(p_->fd, EPOLLIN, &p_->epoll_task);
  SetTime();
  p_->is_armed = true;
}

void Timer::Stop() {
  if (!p_->is_armed) return;

  struct timespec interval = {0, 0};
  struct timespec value = {0, 0};
  struct itimerspec its = {
      interval,
      value
  };

  int ret = timerfd_settime(p_->fd, 0, &its, NULL);
  if (ret < 0) {
    _DEBUG("%s\n", "Fail to stop timer!");
  }

//  Application::GetInstance()->UnwatchFileDescriptor(p_->fd);

  p_->is_armed = false;
}

void Timer::SetInterval(unsigned int interval) {
  if (p_->interval == interval) return;

  p_->interval = interval;
  if (p_->is_armed) {
    SetTime();
  }
}

unsigned int Timer::GetInterval() const {
  return p_->interval;
}

bool Timer::IsArmed() const {
  return p_->is_armed;
}

uint64_t Timer::GetClockTime() {
  uint64_t retval = 0;
  struct timespec now = {0, 0};

  if (clock_gettime(CLOCK_REALTIME, &now) < 0) {
    _DEBUG("%s\n", "Error! Cannot get clock time!");
    return retval;
  }

  retval = (uint64_t) now.tv_sec * 1000 * 1000 * 1000 + now.tv_nsec;
  return retval;
}

bool Timer::SetTime() {
  int ret = 0;
  unsigned int sec = p_->interval / 1000000;
  long nsec = (p_->interval % 1000000) * 1000;

  struct timespec interval_ts = {sec, nsec};
  struct timespec value_ts = {sec, nsec};
  struct itimerspec its = {
      interval_ts,
      value_ts
  };

  ret = timerfd_settime(p_->fd, 0, &its, NULL);
  if (ret < 0) {
    _DEBUG("%s\n", "Fail to set timer!");
    return false;
  }

  return true;
}

} // namespace gui
} // namespace wiztk
