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

#include <stdexcept>
#include "wiztk/system/threading/thread.hpp"

namespace wiztk {
namespace system {
namespace threading {

/**
 * @brief Private structure in Thread.
 */
struct Thread::Private {

  /**
   * @brief A static helper method to start a thread routine.
   * @param thread
   * @return
   */
  static void *StartThread(Thread *thread);

  /**
   * @brief A static helper method to start a runnable routine.
   * @param runnable
   * @return
   */
  static void *StartRunnable(AbstractRunnable *runnable);

};

void *Thread::Private::StartThread(Thread *thread) {
  thread->Run();
  pthread_exit((void *) thread);
}

void *Thread::Private::StartRunnable(AbstractRunnable *runnable) {
  runnable->Run();
  pthread_exit((void *) runnable);
}

// -------

const Thread::DeleterType Thread::kDefaultDeleter = [](AbstractRunnable *obj) { delete obj; };

Thread::~Thread() {
  if (deleter_) deleter_(runnable_);
}

void Thread::Start() {
  typedef void *(*fn)(void *);
  int ret = 0;

  if (runnable_) {
    ret = pthread_create(&id_.native_,
                         nullptr,
                         reinterpret_cast<fn>(Thread::Private::StartRunnable),
                         runnable_);
  } else {
    ret = pthread_create(&id_.native_,
                         nullptr,
                         reinterpret_cast<fn>(Thread::Private::StartThread),
                         this);
  }

  if (ret != 0) throw std::runtime_error("Error! Fail to start a thread!");
}

void Thread::Join() {
  int ret = pthread_join(id_.native_, nullptr);
  if (ret != 0) throw std::runtime_error("Error! Fail to join a thread!");
}

bool operator==(const Thread::ID &id1, const Thread::ID &id2) {
  return 0 != pthread_equal(id1.native_, id2.native_);
}

// -----

Thread::Attribute::Attribute() {
  if (0 != pthread_attr_init(&native_))
    throw std::runtime_error("Error! Cannot initialize thread attribute!");
}

Thread::Attribute::~Attribute() {
  pthread_attr_destroy(&native_);
}

void Thread::Attribute::SetDetachState(DetachStateType state_type) {
  if (0 != pthread_attr_setdetachstate(&native_, state_type))
    throw std::runtime_error("Error! Cannot set detach state!");
}

Thread::Attribute::DetachStateType Thread::Attribute::GetDetachState() const {
  int val = 0;

  if (0 != pthread_attr_getdetachstate(&native_, &val))
    throw std::runtime_error("Error! Cannot get detach state!");

  return static_cast<DetachStateType>(val);
}

void Thread::Attribute::SetScope(ScopeType scope_type) {
  if (0 != pthread_attr_setscope(&native_, scope_type))
    throw std::runtime_error("Error! Cannot set scope!");
}

Thread::Attribute::ScopeType Thread::Attribute::GetScope() const {
  int val = 0;

  if (0 != pthread_attr_getscope(&native_, &val))
    throw std::runtime_error("Error! Cannot get scope!");

  return static_cast<ScopeType>(val);
}

void Thread::Attribute::SetStackSize(size_t stack_size) {
  if (0 != pthread_attr_setstacksize(&native_, stack_size))
    throw std::runtime_error("Error! Cannot set stack size!");
}

size_t Thread::Attribute::GetStackSize() const {
  size_t stack_size = 0;

  if (0 != pthread_attr_getstacksize(&native_, &stack_size))
    throw std::runtime_error("Error! Cannot get stack size!");

  return stack_size;
}

// -----

Thread::Key::Key() {
  if (0 != pthread_key_create(&native_, nullptr))
    throw std::runtime_error("Error! Cannot create pthread key!");
}

Thread::Key::~Key() {
  pthread_key_delete(native_);
}

void Thread::Key::SetSpecific(const void *value) {
  if (0 != pthread_setspecific(native_, value))
    throw std::runtime_error("Error! Cannot set specific!");
}

void *Thread::Key::GetSpecific() const {
  return pthread_getspecific(native_);
}

}  // namespace threading
}  // namespace system
}  // namespace wiztk
