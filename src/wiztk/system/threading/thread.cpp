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

#include "wiztk/system/threading/thread.hpp"

// #include "wiztk/system/event-loop.hpp"

#include <stdexcept>

namespace wiztk {
namespace system {
namespace threading {

Thread::ID Thread::ID::GetCurrent() {
  ID id;
  id.native_ = pthread_self();
  return id;
}

// ----

/**
 * @brief Private structure in Thread.
 */
struct Thread::Private {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);

  Private() = default;

  ~Private() {
//    delete event_loop;
  }

  ID id;

//  EventLoop *event_loop = nullptr;

  Delegate *delegate = nullptr;

  DelegateDeleterType delegate_deleter;

  /**
   * @brief A static helper method to start a thread routine.
   * @param thread
   * @return
   */
  static void *StartRoutine(Thread *thread);

};

ThreadLocal<Thread> Thread::kPerThreadStorage;

void *Thread::Private::StartRoutine(Thread *thread) {
  kPerThreadStorage.Set(thread);

  if (thread->p_->delegate) thread->p_->delegate->Run();
  else thread->Run();

  kPerThreadStorage.Set(nullptr);
  pthread_exit((void *) thread);
}

// -------

const Thread::DelegateDeleterType Thread::kDefaultDelegateDeleter = [](Delegate *obj) {
  delete obj;
};

Thread Thread::kMain(true);

Thread::Thread() {
  p_ = std::make_unique<Private>();
}

Thread::Thread(Delegate *delegate, const DelegateDeleterType &deleter)
    : Thread() {
  p_->delegate = delegate;
  p_->delegate_deleter = deleter;
}

Thread::Thread(const Option &option)
    : Thread() {
  // TODO: process option
}

Thread::Thread(bool /*initialize*/)
    : Thread() {
  p_->id.native_ = pthread_self();
  kPerThreadStorage.Set(this);
}

Thread::~Thread() {
  if (p_->delegate_deleter) p_->delegate_deleter(p_->delegate);
}

void Thread::Start() {
  typedef void *(*fn)(void *);
  int ret = 0;

  ret = pthread_create(&p_->id.native_,
                       nullptr,
                       reinterpret_cast<fn>(Thread::Private::StartRoutine),
                       this);

  if (ret != 0) throw std::runtime_error("Error! Fail to start a thread!");
}

void Thread::Join() {
  if (0 != pthread_join(p_->id.native_, nullptr))
    throw std::runtime_error("Error! Fail to join a thread!");

  p_->id.native_ = pthread_self();
  if (kMain.GetID() == p_->id)
    p_->id.native_ = 0;
}

void Thread::Detach() {
  if (0 != pthread_detach(p_->id.native_))
    throw std::runtime_error("Error! Fail to detach a thread!");
}

const Thread::ID &Thread::GetID() const {
  return p_->id;
}

bool operator==(const Thread::ID &id1, const Thread::ID &id2) {
  return 0 != pthread_equal(id1.native_, id2.native_);
}

bool operator!=(const Thread::ID &id1, const Thread::ID &id2) {
  return 0 == pthread_equal(id1.native_, id2.native_);
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

}  // namespace threading
}  // namespace system
}  // namespace wiztk
