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

#ifndef WIZTK_BASE_DELEGATE_HPP_
#define WIZTK_BASE_DELEGATE_HPP_

#include "wiztk/base/macros.hpp"

#include <cstring>

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

// Forward declarations

// This is the key to use the template format of Delegate<return_type (args...)>
template<typename _Signature>
class Delegate;

template<typename _Signature>
class DelegateRef;

/// @endcond

/**
 * @ingroup base
 * @brief The type of this delegate
 */
enum DelegateType {
  kDelegateTypeUndefined,               /**< The delegate is not bound to any method/function pointer */
  kDelegateTypeMember,                  /**< The delegate is bound to a method (member function) */
  kDelegateTypeStatic                   /**< The delegate is bound to a static function */
};

/**
 * @ingroup base
 * @brief Template class for delegates
 * @tparam ReturnType The return type
 * @tparam ParamTypes Arbitrary number of parameters
 *
 * A delegate is a type that represents references to methods (also called
 * member functions) or static functions with a particular parameter list and
 * return type. Delegate is the key feature and wildely used in WizTK.
 *
 * When you instantiate a delegate, you can associate its instance with any
 * method/function with a compatible signature and return type.
 *
 * The following example shows a delegate to a method:
 *
 * @code
 * class A {
 *  public:
 *   int Foo (int x, int y);
 *   // ...
 * };
 *
 * A a;
 * auto foo = Delegate<int(int, int)>::FromMethod(&a, &A::Foo);
 * @endcode
 *
 * The following example shows a delegate to a static function, in this
 * situation, it works the same as a function pointer:
 *
 * @code
 * class B {
 *  public:
 *   static int Foo (int x, int y);
 *   // ...
 * };
 *
 * int Foo (int x, int y);  // Global function
 *
 * auto foo_b = Delegate<int(int, int)>::FromFunction(&B::Foo);
 * auto foo = Delegate<int(int, int)>::FromFunction(&Foo);
 * @endcode
 *
 * @see <a href="md_doc_delegates.html">Fast C++ Delegats</a>
 */
template<typename ReturnType, typename ... ParamTypes>
class WIZTK_EXPORT Delegate<ReturnType(ParamTypes...)> {

  template<typename ReturnTypeAlias, typename ... ParamTypesAlias>
  friend inline bool operator==(const Delegate<ReturnTypeAlias(ParamTypesAlias...)> &src,
                                const Delegate<ReturnTypeAlias(ParamTypesAlias...)> &dst);

  template<typename ReturnTypeAlias, typename ... ParamTypesAlias>
  friend inline bool operator!=(const Delegate<ReturnTypeAlias(ParamTypesAlias...)> &src,
                                const Delegate<ReturnTypeAlias(ParamTypesAlias...)> &dst);

  template<typename ReturnTypeAlias, typename ... ParamTypesAlias>
  friend inline bool operator<(const Delegate<ReturnTypeAlias(ParamTypesAlias...)> &src,
                               const Delegate<ReturnTypeAlias(ParamTypesAlias...)> &dst);

  template<typename ReturnTypeAlias, typename ... ParamTypesAlias>
  friend inline bool operator>(const Delegate<ReturnTypeAlias(ParamTypesAlias...)> &src,
                               const Delegate<ReturnTypeAlias(ParamTypesAlias...)> &dst);

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

  /**
   * @brief Typedef to static method
   */
  typedef ReturnType (*TFunction)(ParamTypes...);

  /**
   * @brief Create a delegate from the given object and a member function pointer.
   * @tparam T The object type
   * @param object A pointer to an object
   * @param method A pointer to a member function in class T
   * @return A delegate object
   */
  template<typename T>
  static inline Delegate FromMethod(T *object,
                                    ReturnType (T::*method)(ParamTypes...)) {
    return Delegate(object, method);
  }

  /**
   * @brief Create a delegate from the given object and a const member function pointer.
   * @tparam T The object type
   * @param object A pointer to an object
   * @param method A pointer to a member function in class T
   * @return A delegate object
   */
  template<typename T>
  static inline Delegate FromMethod(T *object,
                                    ReturnType (T::*method)(ParamTypes...) const) {
    return Delegate(object, method);
  }

  /**
   * @brief Create a delegate from the given function object.
   * @tparam T A type of function object.
   * @param function A function object such as std::function.
   * @return A delegate object
   *
   * @note Do not create and use delegate to a lambda.
   *
   * When you try to create a delegate to a temporary lambda, for example:
   * @code
   * auto my_delegate = Delegate<int(int, int)>::FromFunction([&](int a, int b) -> int {
   *   return a + b;
   * });
   * @endcode
   *
   * The my_delegate object will be invalid as the lambda here is temporary and
   * will be destructed out of the scope.
   */
  template<typename T>
  static inline Delegate FromFunction(const T &function) {
    typedef ReturnType (T::*TMethod)(ParamTypes...) const;
    TMethod method = &T::operator();
    return FromMethod(const_cast<T *>(&function), method);
  }

  /**
   * @brief Create a delegate from the given static function pointer.
   * @param fn A static function pointer
   * @return A delegate object
   */
  static inline Delegate FromStatic(TFunction fn) {
    return Delegate(fn);
  }

  /**
   * @brief Default constructor
   *
   * Create an empty delegate object, which cannot be called by operator() or
   * Invoke(), and the bool operator returns false.
   */
  Delegate() = default;

  /**
   * @brief Constructor to create a delegate by given object and method
   * @tparam T The type of object
   * @param object A pointer to the object
   * @param method A pointer to a member function in class T
   */
  template<typename T>
  Delegate(T *object, ReturnType (T::*method)(ParamTypes...)) {
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
  Delegate(T *object, ReturnType (T::*method)(ParamTypes...) const) {
    typedef ReturnType (T::*TMethod)(ParamTypes...) const;

    data_.object = object;
    data_.method_stub = &MethodStub<T, TMethod>::invoke;
    data_.pointer.method = reinterpret_cast<internal::GenericMethodPointer>(method);
  }

  explicit Delegate(TFunction fn) {
    data_.object = nullptr;
    data_.method_stub = nullptr;
    data_.pointer.function = reinterpret_cast<void *>(fn);
  }

  /**
   * @brief Copy constructor.
   * @param orig Another delegate
   */
  Delegate(const Delegate &orig)
      : data_(orig.data_) {}

  /**
   * @brief Move constructor.
   * @param other Other delegate
   */
  Delegate(Delegate &&other) noexcept
      : data_(other.data_) {
    other.Reset();
  }

  /**
   * @brief Destructor
   */
  ~Delegate() = default;

  /**
   * @brief Assignment operator overloading
   * @param orig Another delegate
   * @return
   */
  Delegate &operator=(const Delegate &orig) {
    data_ = orig.data_;
    return *this;
  }

  /**
   * @brief Move operator
   * @param other
   * @return
   *
   * @note It's not thread safe to move a deleate in multiple threads.
   */
  Delegate &operator=(Delegate &&other) noexcept {
    data_ = other.data_;
    other.Reset();
    return *this;
  };

  /**
   * @brief Re-assign this delegate to a static function
   * @param fn Static function pointer, use nullptr to reset this delegate, same as Reset()
   * @return Reference to this delegate
   *
   */
  Delegate &operator=(TFunction fn) {
    data_.object = nullptr;
    data_.method_stub = nullptr;
    data_.pointer.function = reinterpret_cast<void *>(fn);
    return *this;
  }

  /**
   * @brief Invoke the method/function bound to this delegate.
   * @param Args
   * @return
   */
  ReturnType operator()(ParamTypes... Args) const {
    if (data_.object) {
      _ASSERT(nullptr != data_.method_stub);
      return (*data_.method_stub)(data_.object, data_.pointer.method, Args...);
    }

    _ASSERT(nullptr == data_.method_stub);
    return reinterpret_cast<TFunction >(data_.pointer.function)(Args...);
  }

  /**
   * @brief Invoke the method/function bound to this delegate.
   * @param Args
   * @return
   *
   * @note For method, the delegate does not check if the object is deleted.
   */
  ReturnType Invoke(ParamTypes... Args) const {
    if (data_.object) {
      _ASSERT(nullptr != data_.method_stub);
      return (*data_.method_stub)(data_.object, data_.pointer.method, Args...);
    }

    _ASSERT(nullptr == data_.method_stub);
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
   * @note After reset, invoke this delegate by operator() or Invoke() will
   * cause segment fault.  The bool operator will return false.
   */
  void Reset() {
    memset(&data_, 0, sizeof(Data));
  }

  /**
   * @brief Compare this delegate to a member function of an object
   * @tparam T
   * @param object
   * @param method
   * @return
   */
  template<typename T>
  bool Equal(T *object, ReturnType(T::*method)(ParamTypes...)) const {
    typedef ReturnType (T::*TMethod)(ParamTypes...);

    return (data_.object == object) &&
        (data_.method_stub == &MethodStub<T, TMethod>::invoke) &&
        (data_.pointer.method == reinterpret_cast<internal::GenericMethodPointer>(method));
  }

  /**
   * @brief Compare this delegate to a const member function of an object.
   * @tparam T
   * @param object
   * @param method
   * @return
   */
  template<typename T>
  bool Equal(T *object, ReturnType(T::*method)(ParamTypes...) const) const {
    typedef ReturnType (T::*TMethod)(ParamTypes...) const;

    return (data_.object == object) &&
        (data_.method_stub == &MethodStub<T, TMethod>::invoke) &&
        (data_.pointer.method == reinterpret_cast<internal::GenericMethodPointer>(method));
  }

  /**
   * @brief Compare this delegate to a lambda.
   * @tparam T
   * @param function
   * @return
   */
  template<typename T>
  bool Equal(const T &function) {
    typedef ReturnType (T::*TMethod)(ParamTypes...) const;

    TMethod method = &T::operator();
    auto *object = const_cast<T *> (&function);

    return (data_.object == object) &&
        (data_.method_stub == &MethodStub<T, TMethod>::invoke) &&
        (data_.pointer.method == reinterpret_cast<internal::GenericMethodPointer>(method));
  }

  /**
   * @brief Compare this delegate to a static function
   * @param fn
   * @return
   */
  bool EqualStatic(TFunction fn) const {
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

  /**
   * @brief Returns the type of this delegate
   * @return One of DelegateType
   */
  DelegateType type() const {
    if (nullptr == data_.object) {
      _ASSERT(nullptr == data_.method_stub);
      return nullptr == data_.pointer.function ? kDelegateTypeUndefined : kDelegateTypeStatic;
    }

    _ASSERT(nullptr != data_.method_stub);
    return kDelegateTypeMember;
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
inline bool operator==(const Delegate<ReturnType(ParamTypes...)> &src,
                       const Delegate<ReturnType(ParamTypes...)> &dst) {
  return memcmp(&src.data_, &dst.data_,
                sizeof(typename Delegate<ReturnType(ParamTypes...)>::Data)
  ) == 0;
}

template<typename ReturnType, typename ... ParamTypes>
inline bool operator!=(const Delegate<ReturnType(ParamTypes...)> &src,
                       const Delegate<ReturnType(ParamTypes...)> &dst) {
  return memcmp(&src.data_, &dst.data_,
                sizeof(typename Delegate<ReturnType(ParamTypes...)>::Data)
  ) != 0;
}

template<typename ReturnType, typename ... ParamTypes>
inline bool operator<(const Delegate<ReturnType(ParamTypes...)> &src,
                      const Delegate<ReturnType(ParamTypes...)> &dst) {
  return memcmp(&src.data_, &dst.data_,
                sizeof(typename Delegate<ReturnType(ParamTypes...)>::Data)
  ) < 0;
}

template<typename ReturnType, typename ... ParamTypes>
inline bool operator>(const Delegate<ReturnType(ParamTypes...)> &src,
                      const Delegate<ReturnType(ParamTypes...)> &dst) {
  return memcmp(&src.data_, &dst.data_,
                sizeof(typename Delegate<ReturnType(ParamTypes...)>::Data)
  ) > 0;
}

/**
 * @ingroup base
 * @brief A reference to a corresponding delegate
 */
template<typename ReturnType, typename ... ParamTypes>
class WIZTK_EXPORT DelegateRef<ReturnType(ParamTypes...)> {

 public:

  /**
   * @brief Typedef to a static function.
   */
  typedef ReturnType (*TFunction)(ParamTypes...);

  DelegateRef() = delete;
  DelegateRef &operator=(const DelegateRef &) = delete;
  DelegateRef(const DelegateRef &) = delete;
  DelegateRef &operator=(DelegateRef &&) = delete;

  /**
   * @brief Constructs a DelegateRef with the given Delegate.
   * @param delegate
   */
  DelegateRef(Delegate<ReturnType(ParamTypes...)> &delegate)
      : delegate_(&delegate) {}

  /**
   * @brief Move constructor.
   * @param orig
   */
  DelegateRef(DelegateRef &&orig) noexcept
      : delegate_(orig.delegate_) {
    orig.delegate_ = nullptr;
  }

  /**
   * @brief Default destructor.
   */
  ~DelegateRef() = default;

  /**
   * @brief Assignment operator.
   * @param delegate A Delegate object
   * @return Reference to this object
   */
  DelegateRef &operator=(Delegate<ReturnType(ParamTypes...)> &delegate) {
    delegate_ = &delegate;
    return *this;
  }

  /**
   * @brief Bind the delegate referenced to given method.
   * @tparam T
   * @param obj
   * @param method
   */
  template<typename T>
  void Bind(T *obj, ReturnType (T::*method)(ParamTypes...)) {
    *delegate_ = Delegate<ReturnType(ParamTypes...)>::template FromMethod<T>(obj, method);
  }

  /**
   * @brief Bind the delegate referenced to given const method.
   * @tparam T
   * @param obj
   * @param method
   */
  template<typename T>
  void Bind(T *obj, ReturnType (T::*method)(ParamTypes...) const) {
    *delegate_ = Delegate<ReturnType(ParamTypes...)>::template FromMethod<T>(obj, method);
  }

  /**
   * @brief Bind the delegate referenced to given lambda.
   * @tparam T
   * @param function
   */
  template<typename T>
  void Bind(const T &function) {
    *delegate_ = Delegate<ReturnType(ParamTypes...)>::template FromFunction(function);
  }

  /**
   * @brief Bind the delegate referenced to given static function.
   * @param fn
   */
  void BindStatic(TFunction fn) {
    *delegate_ = Delegate<ReturnType(ParamTypes...)>::FromStatic(fn);
  }

  /**
   * @brief Reset the delegate referenced.
   */
  void Reset() {
    delegate_->Reset();
  }

  /**
   * @brief Returns if the delegate referenced points to a method.
   * @tparam T
   * @param obj
   * @param method
   * @return
   */
  template<typename T>
  bool IsBoundTo(T *obj, ReturnType (T::*method)(ParamTypes...)) const {
    return delegate_->Equal(obj, method);
  }

  /**
   * @brief Returns if the delegate referenced points to a const method.
   * @tparam T
   * @param obj
   * @param method
   * @return
   */
  template<typename T>
  bool IsBoundTo(T *obj, ReturnType (T::*method)(ParamTypes...) const) const {
    return delegate_->Equal(obj, method);
  }

  template<typename T>
  bool IsBoundTo(const T &function) const {
    return delegate_->Equal(function);
  }

  /**
    * @brief Returns if the delegate referenced points to a static function.
    * @param fn
    * @return
    */
  bool IsBoundToStatic(TFunction fn) const {
    return delegate_->EqualStatic(fn);
  }

  /**
   * @brief Override the bool operation.
   * @return
   */
  explicit operator bool() const {
    return delegate_->operator bool();
  }

 private:

  Delegate<ReturnType(ParamTypes...)> *delegate_;

};

} // namespace base
} // namespace wiztk

#endif  // WIZTK_BASE_DELEGATE_HPP_
