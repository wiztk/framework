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

#ifndef WIZTK_CORE_MEMORY_HPP_
#define WIZTK_CORE_MEMORY_HPP_

#include "macros.hpp"

#include <memory>
#include <atomic>

namespace wiztk {
namespace base {

/**
 * @ingroup base
 * @brief The base class for objects that may be shared by multiple objects.
 *
 * SPCountedBase is the base class to be used in SharedPtr and WeakPtr for
 * implementing our custom reference counting smart pointer. Though we can use
 * std::shared_ptr to store any type of object, there still be situation that we
 * need to indicate what kind of object can be used in a shared pointer.
 *
 * You usually define your own sub class of SPCountedBase, create and contain an
 * instance in a SharedPtr. Use WeakPtr to avoid curcular references in
 * appropriate time.
 *
 * Each SPCountedBase object has a pointer to a Counter object which stores 2
 * different counters:
 *
 *   - use_count: the number of SharedPtr instances pointing to this
 *     SPCountedBase object
 *
 *   - weak_count: the number of WeakPtr instances pointing to this
 *     SPCountedBase object
 *
 * A SPCountedBase object knows its reference status by checking the counters by
 * use_count() or weak_count(). When a SPCountedBase object is created by new
 * operator and not controled by any SharedPtr or WeakPtr, both use_count and
 * weak_count are zero, in this case you can safely delete the
 * instance. Otherwise, it must be destroyed by a SharedPtr.
 *
 * A SPCountedBase object is non-copyable and non-movable.
 */
class RefCountedBase {

  template<typename T> friend
  class SharedPtr;

  template<typename T> friend
  class WeakPtr;

 public:

  /**
   * @brief A helper structure to store counters
   */
  struct Counter {

    Counter()
        : use_count(0), weak_count(0) {
#ifdef UNIT_TEST
      _DEBUG("%s\n", __func__);
#endif  // UNIT_TEST
    }

    ~Counter() {
#ifdef UNIT_TEST
      _DEBUG("%s\n", __func__);
#endif  // UNIT_TEST
    }

    /**
     * @brief The number of SharedPtr instances pointing to the object
     */
    std::atomic_ulong use_count;

    /**
     * @brief The number of WeakPtr instances pointing to the object, plus one
     * if the "use_count" is still > 0
     */
    std::atomic_ulong weak_count;

  };

  /**
   * @brief Disable copy and move constructors
   */
  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(RefCountedBase);

  /**
   * @brief Default constructor
   */
  RefCountedBase()
      : counter_(new Counter) {}

  /**
   * @brief Destructor
   *
   * The destructor will try to delete the internal counter object, but in most
   * cases it's deleted by SharedPtr or WeakPtr.
   */
  virtual ~RefCountedBase() {
#ifdef UNIT_TEST
    if (nullptr != counter_) {
      _DEBUG("use_count: %ld, weak_count: %ld\n",
             counter_->use_count.load(),
             counter_->weak_count.load());
    }
#endif  // TEST
    delete counter_;
  }

  /**
   * @brief Get the use count
   */
  size_t use_count() const {
    return counter_->use_count.load();
  }

  /**
   * @brief Get the weak count
   */
  size_t weak_count() const {
    return counter_->weak_count.load();
  }

 private:

  Counter *counter_;

};

/**
 * @ingroup base
 * @brief Shared pointer
 * @tparam T Must be SPCountedBase or the sub class
 */
template<typename T>
class SharedPtr {

  template<typename R> friend
  class WeakPtr;

  template<typename R>
  friend void Swap(SharedPtr<R> &src, SharedPtr<R> &dst);

 public:

  /**
   * @brief Default constructor
   */
  constexpr SharedPtr() noexcept = default;

  /**
   * @brief Constructor with an assigned SPCountedBase object
   * @param[in] obj A SPCountedBase object, must not be nullptr
   *
   * This constructor will increase the use count of obj.
   */
  explicit SharedPtr(T *obj) noexcept
      : ptr_(obj) {
    counter_ = ptr_->counter_;
    ++counter_->use_count;
  }

  /**
   * @brief Copy constructor
   *
   * Increase the use count if the object in orig is not nullptr.
   */
  SharedPtr(const SharedPtr &orig) noexcept
      : ptr_(orig.ptr_) {
    if (nullptr != ptr_) {
      counter_ = ptr_->counter_;
      ++counter_->use_count;
    }
  }

  /**
   * @brief Move constructor
   * @param orig
   */
  SharedPtr(SharedPtr &&orig) noexcept
      : ptr_(orig.ptr_), counter_(orig.counter_) {
    orig.ptr_ = nullptr;
    orig.counter_ = nullptr;
  }

  /**
   * @brief Destructor
   *
   * When a SharedPtr object is destroyed, it will decrease the use count and
   * delete the object it points to if use count == 0.
   */
  ~SharedPtr() {
    if (nullptr != counter_) {
      _ASSERT((nullptr != ptr_) && (ptr_->counter_ == counter_));

      --counter_->use_count;
      if (0 == counter_->use_count) {
        ptr_->counter_ = nullptr; // set this value to avoid destroying the counter

        // Important: increase the weak count before delete the pointer, then
        // decrease it to avoid double delete counter_ in cross-reference
        // situation:
        ++counter_->weak_count;
        delete ptr_;
        --counter_->weak_count;

        if (0 == counter_->weak_count) delete counter_;
      }
    } else {
      _ASSERT(nullptr == ptr_);
    }
  }

  /**
   * @brief Copy assignment
   * @param other
   * @return
   *
   * The copy assignments adds the object as a shared owner of other's assets,
   * increasing the use_count.
   */
  SharedPtr &operator=(const SharedPtr &other) noexcept {
    Reset(other.ptr_);
    return *this;
  }

  /**
   * @brief Move assignment
   * @param other
   * @return
   */
  SharedPtr &operator=(SharedPtr &&other) noexcept {
    ptr_ = other.ptr_;
    counter_ = other.counter_;
    other.ptr_ = nullptr;
    other.counter_ = nullptr;
    return *this;
  }

  /**
   * @brief Assignment overloading
   *
   * Adds this object as a shared owner of obj, increasing the use_count.
   */
  SharedPtr &operator=(T *obj) noexcept {
    Reset(obj);
    return *this;
  }

  /**
   * @brief Swap content
   * @param other
   */
  void Swap(SharedPtr &other) noexcept {
    T *object = ptr_;
    RefCountedBase::Counter *counter = counter_;

    ptr_ = other.ptr_;
    counter_ = other.counter_;

    other.ptr_ = object;
    other.counter_ = counter;
  }

  /**
   * @brief Reset pointer
   * @param obj
   */
  void Reset(T *obj = nullptr) noexcept {
    T *old_object = ptr_;
    RefCountedBase::Counter *old_counter = counter_;

    ptr_ = obj;
    counter_ = nullptr;
    if (nullptr != ptr_) {
      counter_ = ptr_->counter_;
      ++counter_->use_count;
    }

    if (nullptr != old_counter) {
      _ASSERT((nullptr != old_object) && (old_object->counter_ == old_counter));

      --old_counter->use_count;
      if (0 == old_counter->use_count) {
        old_object->counter_ = nullptr;

        // Important: avoid double delete old_counter:
        ++old_counter->weak_count;
        delete old_object;
        --old_counter->weak_count;

        if (0 == old_counter->weak_count) delete old_counter;
      }
    } else {
      _ASSERT(nullptr == old_object);
    }
  }

  /**
   * @brief Get pointer
   * @return
   */
  T *Get() const noexcept {
    return ptr_;
  }

  /**
   * @brief Dereference object
   * @return
   */
  T &operator*() const noexcept {
    return *ptr_;
  }

  /**
   * @brief Dereference object member
   * @return
   */
  T *operator->() const noexcept {
    return ptr_;
  }

  /**
   * @brief Get the use count
   * @return
   *
   * Returns the number of SharedPtr objects that share ownership over the same
   * pointer as this object (including it).
   *
   * If this is an empty SharedPtr, the function returns zero.
   */
  size_t use_count() const noexcept {
    return nullptr == counter_ ? 0 : counter_->use_count.load();
  }

  /**
   * @brief Get the weak count
   */
  size_t weak_count() const noexcept {
    return nullptr == counter_ ? 0 : counter_->weak_count.load();
  }

  /**
   * @brief Returns whether this shared pointer does not share ownership over
   * its pointer with other.
   * @return
   */
  bool IsUnique() const noexcept {
    return 1 == use_count();
  }

  /**
   * @brief bool operator
   */
  explicit operator bool() const noexcept {
    if (nullptr == ptr_) {
      _ASSERT(nullptr == counter_);
      return false;
    }

    _ASSERT(ptr_->counter_ == counter_);
    return true;
  }

 private:

  T *ptr_ = nullptr;

  RefCountedBase::Counter *counter_ = nullptr;

};

/**
 * @ingroup base
 * @brief Weak ptr
 * @tparam T
 */
template<typename T>
class WeakPtr {

  template<typename R>
  friend void Swap(WeakPtr<R> &src, WeakPtr<R> &dst);

 public:

  /**
   * @brief Default constructor
   */
  constexpr WeakPtr() noexcept = default;

  /**
   * @brief Copy constructor
   */
  WeakPtr(const WeakPtr &orig) noexcept {
    ptr_ = orig.ptr_;
    counter_ = orig.counter_;
    if (nullptr != counter_) ++counter_->weak_count;
  }

  /**
   * @brief Constructor by a SharedPtr
   */
  explicit WeakPtr(const SharedPtr<T> &orig) noexcept {
    ptr_ = orig.ptr_;
    counter_ = orig.counter_;
    if (nullptr != counter_) ++counter_->weak_count;
  }

  /**
   * @brief Destructor
   */
  ~WeakPtr() {
    if (nullptr != counter_) {
      --counter_->weak_count;
      if ((0 == counter_->use_count) && (0 == counter_->weak_count)) {
        delete counter_;
      }
    }
  }

  /**
   * @brief Assignment from a SharedPtr
   */
  WeakPtr &operator=(const SharedPtr<T> &other) noexcept {
    RefCountedBase::Counter *old_counter = counter_;

    ptr_ = other.ptr_;
    counter_ = other.counter_;

    if (nullptr != counter_) ++counter_->weak_count;

    if (nullptr != old_counter) {
      --old_counter->weak_count;
      if ((0 == old_counter->use_count) && (0 == old_counter->weak_count)) {
        delete old_counter;
      }
    }

    return *this;
  }

  /**
   * @brief Copy assignment
   */
  WeakPtr &operator=(const WeakPtr &other) noexcept {
    RefCountedBase::Counter *old_counter = counter_;

    ptr_ = other.ptr_;
    counter_ = other.counter_;

    if (nullptr != counter_) ++counter_->weak_count;

    if (nullptr != old_counter) {
      --old_counter->weak_count;
      if ((0 == old_counter->use_count) && (0 == old_counter->weak_count)) {
        delete old_counter;
      }
    }

    return *this;
  }

  /**
   * @brief Reset pointer
   */
  void Reset() noexcept {
    ptr_ = nullptr;
    if (nullptr != counter_) {
      --counter_->weak_count;
      if ((0 == counter_->use_count) && (0 == counter_->weak_count)) {
        delete counter_;
      }
      counter_ = nullptr;
    }
  }

  /**
   * @brief Get use count
   */
  size_t use_count() const noexcept {
    return nullptr == counter_ ? 0 : counter_->use_count.load();
  }

  /**
   * @brief Get weak count
   */
  size_t weak_count() const noexcept {
    return nullptr == counter_ ? 0 : counter_->weak_count.load();
  }

  /**
   * @brief Returns a SharedPtr with the object preserved if it is not expired.
   *
   * When you try to obtain a SharedPtr by this method, the WeakPtr checks the
   * "use count", and if it's > 0 returns a valid SharedPtr . If the "use count"
   * was already zero you get an empty SharedPtr instance instead.
   */
  SharedPtr<T> Lock() const noexcept {
    if (counter_->use_count > 0) {
      return SharedPtr<T>(ptr_);
    }

    return SharedPtr<T>();
  }

  /**
   * @brief Bool operator
   * @return
   */
  explicit operator bool() const noexcept {
    if (nullptr != counter_) {
      _ASSERT(0 != counter_->use_count || 0 != counter_->weak_count);
      return true;
    }
    
    return false;
  }

 private:

  T *ptr_ = nullptr;

  RefCountedBase::Counter *counter_ = nullptr;

};

/**
 * @ingroup base
 * @brief Constructs an object of tyep T and wraps it in a SharedPtr
 */
template<typename T, typename ... Args>
SharedPtr<T> MakeShared(Args &&... args) {
  return SharedPtr<T>(new T(std::forward<Args>(args)...));
};

/**
 * @ingroup base
 * @brief Exchange content of SharedPtr objects
 */
template<typename T>
void Swap(SharedPtr<T> &src, SharedPtr<T> &dst) {
  T *object = src.ptr_;
  RefCountedBase::Counter *counter = src.counter_;

  src.ptr_ = dst.ptr_;
  src.counter_ = dst.counter_;

  dst.ptr_ = object;
  dst.counter_ = counter;
}

/**
 * @ingroup base
 * @brief Exchange content of WeakPtr objects
 */
template<typename T>
void Swap(WeakPtr<T> &src, WeakPtr<T> &dst) {
  T *object = src.ptr_;
  RefCountedBase::Counter *counter = src.counter_;

  src.ptr_ = dst.ptr_;
  src.counter_ = dst.counter_;

  dst.ptr_ = object;
  dst.counter_ = counter;
}

} // namespace base
} // namespace wiztk

#endif // WIZTK_CORE_MEMORY_HPP_
