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

#ifndef WIZTK_BASE_TIMER_HPP_
#define WIZTK_BASE_TIMER_HPP_

#include "wiztk/base/macros.hpp"
#include "wiztk/base/delegate.hpp"

#include <csignal>
#include <ctime>
#include <cstdint>

namespace wiztk {
namespace base {

/**
 * @ingroup base
 * @brief A wrapper class to use posix timer
 */
class WIZTK_EXPORT Timer {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Timer);

  Timer();

  ~Timer();

  /**
   * @brief Start the posix timer
   */
  void Start();

  void Stop();

  void SetInterval(unsigned int interval);

  unsigned int interval() const {
    return interval_;
  }

  bool is_armed() const {
    return is_armed_;
  }

  /**
   * Timeout delegate
   * @return Delegate reference
   *
   * @warning This delegate was called in thread, and SHOULD not be reset when the timer is armed.
   */
  DelegateRefT<void()> expire() { return expire_; }

 protected:

  static void OnExpire(union sigval sigev_value);

 private:

  timer_t Create();

  bool SetTime();

  timer_t id_;

  unsigned int interval_;

  bool is_armed_;

  DelegateT<void()> expire_;

};

} // namespace base
} // namespace wiztk

#endif // WIZTK_CORE_TIMER_HPP_
