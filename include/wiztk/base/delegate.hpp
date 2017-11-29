/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Freeman Zhang <zhanggyb@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef WIZTK_BASE_DELEGATE_HPP_
#define WIZTK_BASE_DELEGATE_HPP_

#include <cstring>
#include <functional>

#ifdef __DEBUG__
#include <cassert>
#endif

namespace wiztk {
namespace base {

/// @cond IGNORE

namespace internal {

// generic classes to calculate method pointer:
class GenericBase1 {};
class GenericBase2 {};
class GenericMultiInherit : GenericBase1, GenericBase2 {};

typedef void (GenericMultiInherit::*GenericMethodPointer)();

} // namespace internal

// A forward declaration
// This is the key to use the template format of Delegate<return_type (args...)>
template<typename _Signature>
class DelegateT;

template<typename _Signature>
class DelegateRefT;

/// @endcond

/**
 * @ingroup base
 * @brief Template class for delegates
 * @tparam ReturnType The return type
 * @tparam ParamTypes Parameters
 *
 * @see <a href="md_doc_delegates.html">Fast C++ Delegats</a>
 */
template<typename ReturnType, typename ... ParamTypes>
class DelegateT<ReturnType(ParamTypes...)> {

  template<typename ReturnTypeAlias, typename ... ParamTypesAlias>
  friend inline bool operator==(const DelegateT<ReturnTypeAlias(ParamTypesAlias...)> &src,
                                const DelegateT<ReturnTypeAlias(ParamTypesAlias...)> &dst);

  template<typename ReturnTypeAlias, typename ... ParamTypesAlias>
  friend inline bool operator!=(const DelegateT<ReturnTypeAlias(ParamTypesAlias...)> &src,
                                const DelegateT<ReturnTypeAlias(ParamTypesAlias...)> &dst);

  template<typename ReturnTypeAlias, typename ... ParamTypesAlias>
  friend inline bool operator<(const DelegateT<ReturnTypeAlias(ParamTypesAlias...)> &src,
                               const DelegateT<ReturnTypeAlias(ParamTypesAlias...)> &dst);

  template<typename ReturnTypeAlias, typename ... ParamTypesAlias>
  friend inline bool operator>(const DelegateT<ReturnTypeAlias(ParamTypesAlias...)> &src,
                               const DelegateT<ReturnTypeAlias(ParamTypesAlias...)> &dst);

  typedef ReturnType (*MethodStubType)(void *object, internal::GenericMethodPointer, ParamTypes...);

  struct Data {

    Data()
        : object(nullptr),
          method_stub(nullptr) {
      pointer.method = nullptr;
    }

    Data(const Data &other)
        : object(other.object),
          method_stub(other.method_stub),
          pointer(other.pointer) {}

    Data(Data &&) = delete;

    Data &operator=(const Data &other) {
      object = other.object;
      method_stub = other.method_stub;
      pointer = other.pointer;
      return *this;
    }

    Data &operator=(Data &&) = delete;

    void *object;
    MethodStubType method_stub;
    union {
      internal::GenericMethodPointer method;  // member function pointer
      void *function; // static function pointer
    } pointer;

  };

  template<typename T, typename TFxn>
  struct MethodStub {
    static ReturnType invoke(void *object, internal::GenericMethodPointer any, ParamTypes ... Args) {
      auto *obj = static_cast<T *>(object);
      return (obj->*reinterpret_cast<TFxn>(any))(Args...);
    }
  };

 public:

  typedef ReturnType (*TFunction)(ParamTypes...);

  /**
   * @brief Create a delegate from the given object and a member function pointer.
   * @tparam T The object type
   * @param object A pointer to an object
   * @param method A pointer to a member function in class T
   * @return A delegate object
   */
  template<typename T>
  static inline DelegateT FromMethod(T *object,
                                     ReturnType (T::*method)(ParamTypes...)) {
    return DelegateT(object, method);
  }

  /**
   * @brief Create a delegate from the given object and a const member function pointer.
   * @tparam T The object type
   * @param object A pointer to an object
   * @param method A pointer to a member function in class T
   * @return A delegate object
   */
  template<typename T>
  static inline DelegateT FromMethod(T *object,
                                     ReturnType (T::*method)(ParamTypes...) const) {
    return DelegateT(object, method);
  }

  /**
   * @brief Create a delegate from the given static function pointer.
   * @param fn A static function pointer
   * @return A delegate object
   */
  static inline DelegateT FromFunction(TFunction fn) {
    return DelegateT(fn);
  }

  /**
   * @brief Default constructor
   *
   * Create an empty delegate object, which cannot be called by operator() or
   * Invoke(), and the bool operator returns false.
   */
  DelegateT() = default;

  /**
   * @brief Constructor to create a delegate by given object and method
   * @tparam T The type of object
   * @param object A pointer to the object
   * @param method A pointer to a member function in class T
   */
  template<typename T>
  DelegateT(T *object, ReturnType (T::*method)(ParamTypes...)) {
    typedef ReturnType (T::*TMethod)(ParamTypes...);

    data_.object = object;
    data_.method_stub = &MethodStub<T, TMethod>::invoke;
    data_.pointer.method = reinterpret_cast<internal::GenericMethodPointer>(method);
  }

  /**
   * @brief Constructor to create a delegate by given object and const method
   * @tparam T The type of object
   * @param object A pointer to the object
   * @param method A pointer to a const member function in class T
   */
  template<typename T>
  DelegateT(T *object, ReturnType (T::*method)(ParamTypes...) const) {
    typedef ReturnType (T::*TMethod)(ParamTypes...) const;

    data_.object = object;
    data_.method_stub = &MethodStub<T, TMethod>::invoke;
    data_.pointer.method = reinterpret_cast<internal::GenericMethodPointer>(method);
  }

  explicit DelegateT(TFunction fn) {
    data_.object = nullptr;
    data_.method_stub = nullptr;
    data_.pointer.function = reinterpret_cast<void *>(fn);
  }

  /**
   * @brief Copy constructor
   * @param orig Another delegate
   */
  DelegateT(const DelegateT &orig)
      : data_(orig.data_) {}

  DelegateT(DelegateT &&other) noexcept
      : data_(other.data_) {}

  /**
   * @brief Destructor
   */
  ~DelegateT() = default;

  /**
   * @brief Assignment operator overloading
   * @param orig Another delegate
   * @return
   */
  DelegateT &operator=(const DelegateT &orig) {
    data_ = orig.data_;
    return *this;
  }

  DelegateT &operator=(DelegateT &&other) noexcept {
    data_ = other.data_;
    return *this;
  };

  /**
   * @brief Re-assign this delegate to a static function
   * @param fn Static function pointer, use nullptr to reset this delegate, same as Reset()
   * @return Reference to this delegate
   *
   */
  DelegateT &operator=(TFunction fn) {
    data_.object = nullptr;
    data_.method_stub = nullptr;
    data_.pointer.function = reinterpret_cast<void *>(fn);
    return *this;
  }

  ReturnType operator()(ParamTypes... Args) const {
    if (data_.object) {
#ifdef __DEBUG__
      assert(nullptr != data_.method_stub);
#endif
      return (*data_.method_stub)(data_.object, data_.pointer.method, Args...);
    }

#ifdef __DEBUG__
    assert(nullptr == data_.method_stub);
#endif
    return reinterpret_cast<TFunction >(data_.pointer.function)(Args...);
  }

  ReturnType Invoke(ParamTypes... Args) const {
    if (data_.object) {
#ifdef __DEBUG__
      assert(nullptr != data_.method_stub);
#endif
      return (*data_.method_stub)(data_.object, data_.pointer.method, Args...);
    }

#ifdef __DEBUG__
    assert(nullptr == data_.method_stub);
#endif
    return reinterpret_cast<TFunction >(data_.pointer.function)(Args...);
  }

  /**
   * @brief Bool operator
   * @return True if pointer to a method is set, false otherwise
   */
  explicit operator bool() const {
    return nullptr != data_.pointer.method;
  }

  /**
   * @brief Reset this delegate
   *
   * Reset pointers in this delegate to nullptr (0)
   *
   * @note After reset, invoke this delegate by operator() or Invoke() will cause segment fault.
   * The bool operator will return false.
   */
  void Reset() {
    memset(&data_, 0, sizeof(Data));
  }

  /**
   * @brief Compare this delegate to a member function of an object
   * @tparam T
   * @param object_ptr
   * @param method
   * @return
   */
  template<typename T>
  bool Equal(T *object_ptr, ReturnType(T::*method)(ParamTypes...)) const {
    typedef ReturnType (T::*TMethod)(ParamTypes...);

    return (data_.object == object_ptr) &&
        (data_.method_stub == &MethodStub<T, TMethod>::invoke) &&
        (data_.pointer.method == reinterpret_cast<internal::GenericMethodPointer>(method));
  }

  /**
   * @brief Compare this delegate to a const member function of an object
   * @tparam T
   * @param object_ptr
   * @param method
   * @return
   */
  template<typename T>
  bool Equal(T *object_ptr, ReturnType(T::*method)(ParamTypes...) const) const {
    typedef ReturnType (T::*TMethod)(ParamTypes...) const;

    return (data_.object == object_ptr) &&
        (data_.method_stub == &MethodStub<T, TMethod>::invoke) &&
        (data_.pointer.method == reinterpret_cast<internal::GenericMethodPointer>(method));
  }

  bool Equal(TFunction fn) const {
#ifdef __DEBUG__
    if (data_.pointer.function == fn) {
      assert((nullptr == data_.object) && (nullptr == data_.method_stub));
      return true;
    }
    return false;
#else
    return data_.pointer.function == fn;
#endif  // __DEBUG__
  }

 private:

  Data data_;
};

/**
 * @brief Compare 2 delegates
 * @tparam ReturnType
 * @tparam ParamTypes
 * @param src
 * @param dst
 * @return True if 2 delegates point to the same method in one object
 */
template<typename ReturnType, typename ... ParamTypes>
inline bool operator==(const DelegateT<ReturnType(ParamTypes...)> &src,
                       const DelegateT<ReturnType(ParamTypes...)> &dst) {
  return memcmp(&src.data_, &dst.data_,
                sizeof(typename DelegateT<ReturnType(ParamTypes...)>::Data)
  ) == 0;
}

template<typename ReturnType, typename ... ParamTypes>
inline bool operator!=(const DelegateT<ReturnType(ParamTypes...)> &src,
                       const DelegateT<ReturnType(ParamTypes...)> &dst) {
  return memcmp(&src.data_, &dst.data_,
                sizeof(typename DelegateT<ReturnType(ParamTypes...)>::Data)
  ) != 0;
}

template<typename ReturnType, typename ... ParamTypes>
inline bool operator<(const DelegateT<ReturnType(ParamTypes...)> &src,
                      const DelegateT<ReturnType(ParamTypes...)> &dst) {
  return memcmp(&src.data_, &dst.data_,
                sizeof(typename DelegateT<ReturnType(ParamTypes...)>::Data)
  ) < 0;
}

template<typename ReturnType, typename ... ParamTypes>
inline bool operator>(const DelegateT<ReturnType(ParamTypes...)> &src,
                      const DelegateT<ReturnType(ParamTypes...)> &dst) {
  return memcmp(&src.data_, &dst.data_,
                sizeof(typename DelegateT<ReturnType(ParamTypes...)>::Data)
  ) > 0;
}

/**
 * @ingroup base
 * @brief A reference to a corresponding delegate
 */
template<typename ReturnType, typename ... ParamTypes>
class DelegateRefT<ReturnType(ParamTypes...)> {

 public:

  typedef ReturnType (*TFunction)(ParamTypes...);

  DelegateRefT() = delete;
  DelegateRefT &operator=(const DelegateRefT &) = delete;

  DelegateRefT(DelegateT<ReturnType(ParamTypes...)> &delegate)
      : delegate_(&delegate) {}

  DelegateRefT(const DelegateRefT &orig)
      : delegate_(orig.delegate_) {}

  ~DelegateRefT() = default;

  DelegateRefT &operator=(DelegateT<ReturnType(ParamTypes...)> &delegate) {
    delegate_ = &delegate;
    return *this;
  }

  template<typename T>
  void Bind(T *obj, ReturnType (T::*method)(ParamTypes...)) {
    *delegate_ = DelegateT<ReturnType(ParamTypes...)>::template FromMethod<T>(obj, method);
  }

  template<typename T>
  void Bind(T *obj, ReturnType (T::*method)(ParamTypes...) const) {
    *delegate_ = DelegateT<ReturnType(ParamTypes...)>::template FromMethod<T>(obj, method);
  }

  void Bind(TFunction fn) {
    *delegate_ = DelegateT<ReturnType(ParamTypes...)>::FromFunction(fn);
  }

  void Reset() {
    delegate_->Reset();
  }

  template<typename T>
  bool IsAssignedTo(T *obj, ReturnType (T::*method)(ParamTypes...)) const {
    return delegate_->Equal(obj, method);
  }

  template<typename T>
  bool IsAssignedTo(T *obj, ReturnType (T::*method)(ParamTypes...) const) const {
    return delegate_->Equal(obj, method);
  }

  explicit operator bool() const {
    return delegate_->operator bool();
  }

 private:
  DelegateT<ReturnType(ParamTypes...)> *delegate_;
};

} // namespace base
} // namespace wiztk

#endif  // WIZTK_BASE_DELEGATE_HPP_
