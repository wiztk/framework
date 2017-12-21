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

#ifndef WIZTK_BASE_SIGCXX_HPP_
#define WIZTK_BASE_SIGCXX_HPP_

#include "wiztk/base/delegate.hpp"
#include "wiztk/base/deque.hpp"

#include <cstddef>
#ifdef __DEBUG__
#include <cassert>
#endif  // __DEBUG__

#ifndef __SLOT__
/**
 * @ingroup base
 * @brief A helper macro to define a slot parameter with default nullptr
 */
#define __SLOT__ ::wiztk::base::SLOT slot = nullptr
#endif  // __SLOT__

namespace wiztk {
namespace base {

// forward declaration
class Trackable;
class Slot;

template<typename ... ParamTypes>
class Signal;

typedef Slot *SLOT;

/// @cond IGNORE
namespace internal {

struct Token;

template<typename ... ParamTypes>
class SignalToken;

class SlotNode : protected BinodeBase {

 public:

  SlotNode() = default;
  ~SlotNode() override = default;
  SlotNode(SlotNode &&) = default;
  SlotNode &operator=(SlotNode &&) noexcept = default;

  SlotNode(const SlotNode &) noexcept = delete;
  SlotNode &operator=(const SlotNode &) = delete;

  inline void push_back(SlotNode *other) { PushBack(other); }

  inline void push_front(SlotNode *other) { PushFront(other); }

  inline bool is_linked() const { return IsLinked(); }

  inline void unlink() { Unlink(); }

  inline SlotNode *previous() const { return static_cast<SlotNode *>(previous_); }

  inline SlotNode *next() const { return static_cast<SlotNode *> (next_); }

};

/**
 * @brief A simple structure works as a list node in Trackable object
 */
struct Binding {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Binding);

  Binding() = default;

  ~Binding();

  Trackable *trackable = nullptr;
  Binding *previous = nullptr;
  Binding *next = nullptr;
  Token *token = nullptr;

};

/**
 * @brief A simple structure works as a list node in Signal object
 */
struct Token {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Token);

  Token() = default;

  virtual ~Token();

  Trackable *trackable = nullptr;
  Token *previous = nullptr;
  Token *next = nullptr;
  Binding *binding = nullptr;

  SlotNode slot_mark_head;

};

template<typename ... ParamTypes>
class CallableToken : public Token {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(CallableToken);

  CallableToken() = default;

  ~CallableToken() override = default;

  virtual void Invoke(ParamTypes ... Args) {
    // Override this in sub class
  }

};

template<typename ... ParamTypes>
class DelegateToken : public CallableToken<ParamTypes...> {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(DelegateToken);
  DelegateToken() = delete;

  explicit DelegateToken(const Delegate<void(ParamTypes...)> &d)
      : CallableToken<ParamTypes...>(), delegate_(d) {}

  ~DelegateToken() final = default;

  virtual void Invoke(ParamTypes... Args) final {
    delegate_(Args...);
  }

  inline const Delegate<void(ParamTypes...)> &delegate() const {
    return delegate_;
  }

 private:

  Delegate<void(ParamTypes...)> delegate_;

};

template<typename ... ParamTypes>
class SignalToken : public CallableToken<ParamTypes...> {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(SignalToken);
  SignalToken() = delete;

  explicit SignalToken(Signal<ParamTypes...> &signal)
      : CallableToken<ParamTypes...>(), signal_(&signal) {}

  ~SignalToken() final = default;

  virtual void Invoke(ParamTypes... Args) final {
    signal_->Emit(Args...);
  }

  const Signal<ParamTypes...> *signal() const {
    return signal_;
  }

 private:

  Signal<ParamTypes...> *signal_;

};

}// namespace details
/// @endcond

/**
 * @ingroup base
 * @brief Iterator and signature to a slot method
 *
 * A Slot object is created and destroyed when a signal is being emitting.
 *
 * It has two main purposes:
 *   - Works as an iterator
 *   - The last parameter in a slot method
 *
 * A Signal holds a list of token to support multicast, when it's being
 * emitting, it create a simple Slot object and use it as an iterater and call
 * each delegate (@ref Delegate) to the slot method or another signal.
 */
class Slot {

  friend struct internal::Token;
  friend class Trackable;

  template<typename ... ParamTypes> friend
  class Signal;

 public:

  class Mark : public internal::SlotNode {

   public:

    Mark() = delete;

    explicit Mark(Slot *slot)
        : slot_(slot) {}

    ~Mark() final = default;

    Slot *slot() const { return slot_; }

   private:

    Slot *slot_ = nullptr;

  };

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Slot);
  Slot() = delete;

  /**
   * @brief Get the Signal object which is just calling this slot
   */
  template<typename ... ParamTypes>
  Signal<ParamTypes...> *signal() const {
    return dynamic_cast<Signal<ParamTypes...> *>(token_->trackable);
  }

  /**
   * @brief The trackable object in which the slot method is being called
   * @return The trackable object receiving signal
   */
  Trackable *binding_trackable() const {
    return token_->binding->trackable;
  }

 private:

  explicit Slot(internal::Token *token)
      : token_(token), skip_(false), mark_(this) {}

  ~Slot() = default;

  Slot &operator++() {
    token_ = token_->next;
    return *this;
  }

  Slot &operator--() {
    token_ = token_->previous;
    return *this;
  }

  internal::Token *token_;

  bool skip_;

  Mark mark_;

};

/**
 * @ingroup base
 * @brief The basic class for an object which can provide slot methods
 */
class Trackable {

  friend struct internal::Binding;
  friend struct internal::Token;

  template<typename ... ParamTypes> friend
  class Signal;

 public:

  /**
   * @brief Default constructor
   */
  Trackable() = default;

  /**
   * @brief Copy constructor
   *
   * Do nothing in copy constructor.
   */
  Trackable(const Trackable &) {}

  /**
   * @brief Destructor
   */
  virtual ~Trackable();

  /**
   * @brief Copy assignment
   *
   * Do nothing in copy assignment.
   */
  Trackable &operator=(const Trackable &) {
    return *this;
  }

  /**
   * @brief Count connections to the given slot method
   */
  template<typename T, typename ... ParamTypes>
  int CountSignalBindings(void (T::*method)(ParamTypes...)) const;

  /**
   * @brief Count all connections
   */
  int CountSignalBindings() const;

 protected:

  /**
   * @brief Break the connection to a signal by given slot
   *
   * This is the fastest way to disconnect from a signal via the slot parameter.
   */
  void UnbindSignal(SLOT slot);

  /**
    * @brief Break the all connections to this object
    */
  void UnbindAllSignals();

  /**
    * @brief Break all connections to the given slot method of this object
    */
  template<typename T, typename ... ParamTypes>
  void UnbindAllSignalsTo(void (T::*method)(ParamTypes...));

  virtual void AuditDestroyingToken(internal::Token *token) {}

 private:

  void PushBackBinding(internal::Binding *binding);

  void PushFrontBinding(internal::Binding *binding);

  void InsertBinding(int index, internal::Binding *binding);

  static inline void link(internal::Token *token, internal::Binding *binding) {
#ifdef __DEBUG__
    assert((nullptr == token->binding) && (nullptr == binding->token));
#endif

    token->binding = binding;
    binding->token = token;
  }

  static inline void push_front(Trackable *trackable,
                                internal::Binding *binding) {
    trackable->PushBackBinding(binding);
  }

  static inline void push_back(Trackable *trackable,
                               internal::Binding *binding) {
    trackable->PushBackBinding(binding);
  }

  static inline void insert(Trackable *trackable,
                            internal::Binding *binding,
                            int index = 0) {
    trackable->InsertBinding(index, binding);
  }

  internal::Binding *first_binding_ = nullptr;
  internal::Binding *last_binding_ = nullptr;

};

template<typename T, typename ... ParamTypes>
void Trackable::UnbindAllSignalsTo(void (T::*method)(ParamTypes...)) {
  internal::Binding *it = last_binding_;
  internal::Binding *tmp = nullptr;
  internal::DelegateToken<ParamTypes...> *delegate_token = nullptr;

  while (it) {
    tmp = it;
    it = it->previous;

    delegate_token = dynamic_cast<internal::DelegateToken<ParamTypes...> * > (tmp->token);
    if (delegate_token && (delegate_token->delegate().template Equal<T>((T *) this, method))) {
      delete tmp;
    }
  }
}

template<typename T, typename ... ParamTypes>
int Trackable::CountSignalBindings(void (T::*method)(ParamTypes...)) const {
  int count = 0;
  internal::DelegateToken<ParamTypes...> *delegate_token = nullptr;

  for (internal::Binding *p = first_binding_; p; p = p->next) {
    delegate_token = dynamic_cast<internal::DelegateToken<ParamTypes...> * > (p->token);
    if (delegate_token && (delegate_token->delegate().template Equal<T>((T *) this, method))) {
      count++;
    }
  }
  return count;
}

/**
 * @ingroup base
 * @brief A template class which can emit signal(s)
 */
template<typename ... ParamTypes>
class Signal : public Trackable {

  friend class Trackable;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Signal);

  Signal() = default;

  ~Signal() final {
    DisconnectAll();
  }

  /**
   * @brief Connect this signal to a slot method in a observer
   */
  template<typename T>
  void Connect(T *obj, void (T::*method)(ParamTypes..., SLOT), int index = -1);

  void Connect(Signal<ParamTypes...> &other, int index = -1);

  /**
   * @brief Disconnect all delegates to a method
   */
  template<typename T>
  void DisconnectAll(T *obj, void (T::*method)(ParamTypes..., SLOT));

  /**
   * @brief Disconnect all signals
   */
  void DisconnectAll(Signal<ParamTypes...> &other);

  /**
   * @brief Disconnect delegats to a method by given start position and counts
   * @tparam T
   * @param obj
   * @param method
   * @param start_pos
   * @param counts
   * @return A integer number of how many delegates are found and disconnected
   *
   * By the default parameters this disconnect the last delegate to a method.
   */
  template<typename T>
  int Disconnect(T *obj, void (T::*method)(ParamTypes..., SLOT), int start_pos = -1, int counts = 1);

  /**
   * @brief Disconnect connections to a signal by given start position and counts
   * @param other
   * @param start_pos
   * @param counts
   * @return A integer number of how many signals are found and disconnected
   *
   * By the default parameters this disconnect the last signal.
   */
  int Disconnect(Signal<ParamTypes...> &other, int start_pos = -1, int counts = 1);

  /**
   * @brief Disconnect any kind of connections from the start position
   * @param start_pos
   * @param counts How many connections to be break, if it's negative or a very big number,
   *        this is the same as DisconnectAll()
   * @return
   */
  int Disconnect(int start_pos = -1, int counts = 1);

  /**
   * @brief Disconnect all
   */
  void DisconnectAll();

  template<typename T>
  bool IsConnectedTo(T *obj, void (T::*method)(ParamTypes..., SLOT)) const;

  bool IsConnectedTo(const Signal<ParamTypes...> &other) const;

  bool IsConnectedTo(const Trackable *obj) const;

  template<typename T>
  int CountConnections(T *obj, void (T::*method)(ParamTypes..., SLOT)) const;

  int CountConnections(const Signal<ParamTypes...> &other) const;

  int CountConnections() const;

  void Emit(ParamTypes ... Args);

  void operator()(ParamTypes ... Args) {
    Emit(Args...);
  }

 protected:

  void AuditDestroyingToken(internal::Token *token) final;

 private:

  void PushBackToken(internal::Token *token);

  void PushFrontToken(internal::Token *token);

  void InsertToken(int index, internal::Token *token);

  inline internal::Token *first_token() const {
    return first_token_;
  }

  inline internal::Token *last_token() const {
    return last_token_;
  }

  internal::Token *first_token_ = nullptr;
  internal::Token *last_token_ = nullptr;

};

// Signal implementation:

template<typename ... ParamTypes>
template<typename T>
void Signal<ParamTypes...>::Connect(T *obj, void (T::*method)(ParamTypes..., SLOT), int index) {
  internal::Binding *downstream = new internal::Binding;
  Delegate<void(ParamTypes..., SLOT)> d =
      Delegate<void(ParamTypes..., SLOT)>::template FromMethod<T>(obj, method);
  internal::DelegateToken<ParamTypes..., SLOT> *token = new internal::DelegateToken<
      ParamTypes..., SLOT>(d);

  link(token, downstream);
  InsertToken(index, token);
  push_back(obj, downstream);  // always push back binding, don't care about the position in observer
}

template<typename ... ParamTypes>
void Signal<ParamTypes...>::Connect(Signal<ParamTypes...> &other, int index) {
  internal::SignalToken<ParamTypes...> *token = new internal::SignalToken<ParamTypes...>(
      other);
  internal::Binding *binding = new internal::Binding;

  link(token, binding);
  InsertToken(index, token);
  push_back(&other, binding);  // always push back binding, don't care about the position in observer
}

template<typename ... ParamTypes>
template<typename T>
void Signal<ParamTypes...>::DisconnectAll(T *obj, void (T::*method)(ParamTypes..., SLOT)) {
  internal::DelegateToken<ParamTypes..., SLOT> *delegate_token = nullptr;
  internal::Token *it = last_token_;
  internal::Token *tmp = nullptr;

  while (it) {
    tmp = it;
    it = it->previous;

    if (tmp->binding->trackable == obj) {
      delegate_token = dynamic_cast<internal::DelegateToken<ParamTypes..., SLOT> * > (tmp);
      if (delegate_token && (delegate_token->delegate().template Equal<T>(obj, method))) {
        delete tmp;
      }
    }
  }
}

template<typename ... ParamTypes>
void Signal<ParamTypes...>::DisconnectAll(Signal<ParamTypes...> &other) {
  internal::SignalToken<ParamTypes...> *signal_token = nullptr;
  internal::Token *it = last_token_;
  internal::Token *tmp = nullptr;

  while (it) {
    tmp = it;
    it = it->previous;

    if (tmp->binding->trackable == (&other)) {
      signal_token = dynamic_cast<internal::SignalToken<ParamTypes...> * > (tmp);
      if (signal_token && (signal_token->signal() == (&other))) {
        delete tmp;
      }
    }
  }
}

template<typename ... ParamTypes>
template<typename T>
int Signal<ParamTypes...>::Disconnect(T *obj, void (T::*method)(ParamTypes..., SLOT), int start_pos, int counts) {
  internal::DelegateToken<ParamTypes..., SLOT> *delegate_token = nullptr;
  internal::Token *it = nullptr;
  internal::Token *tmp = nullptr;
  int ret_count = 0;

  if (start_pos >= 0) {
    it = first_token_;
    while (it && (start_pos > 0)) {
      it = it->next;
      start_pos--;
    }

    while (it) {
      tmp = it;
      it = it->next;

      if (tmp->binding->trackable == obj) {
        delegate_token = dynamic_cast<internal::DelegateToken<ParamTypes..., SLOT> * > (tmp);
        if (delegate_token && (delegate_token->delegate().template Equal<T>(obj, method))) {
          ret_count++;
          counts--;
          delete tmp;
        }
      }
      if (counts == 0) break;
    }
  } else {
    it = last_token_;
    while (it && (start_pos < -1)) {
      it = it->previous;
      start_pos++;
    }

    while (it) {
      tmp = it;
      it = it->previous;

      if (tmp->binding->trackable == obj) {
        delegate_token = dynamic_cast<internal::DelegateToken<ParamTypes..., SLOT> * > (tmp);
        if (delegate_token && (delegate_token->delegate().template Equal<T>(obj, method))) {
          ret_count++;
          counts--;
          delete tmp;
        }
      }
      if (counts == 0) break;
    }
  }

  return ret_count;
}

template<typename ... ParamTypes>
int Signal<ParamTypes...>::Disconnect(Signal<ParamTypes...> &other, int start_pos, int counts) {
  internal::SignalToken<ParamTypes...> *signal_token = nullptr;
  internal::Token *it = nullptr;
  internal::Token *tmp = nullptr;
  int ret_count = 0;

  if (start_pos >= 0) {
    it = first_token_;
    while (it && (start_pos > 0)) {
      it = it->next;
      start_pos--;
    }

    while (it) {
      tmp = it;
      it = it->next;

      if (tmp->binding->trackable == (&other)) {
        signal_token = dynamic_cast<internal::SignalToken<ParamTypes...> * > (tmp);
        if (signal_token && (signal_token->signal() == (&other))) {
          ret_count++;
          counts--;
          delete tmp;
        }
      }
      if (counts == 0) break;
    }

  } else {
    it = last_token_;
    while (it && (start_pos < -1)) {
      it = it->previous;
      start_pos++;
    }

    while (it) {
      tmp = it;
      it = it->previous;

      if (tmp->binding->trackable == (&other)) {
        signal_token = dynamic_cast<internal::SignalToken<ParamTypes...> * > (tmp);
        if (signal_token && (signal_token->signal() == (&other))) {
          ret_count++;
          counts--;
          delete tmp;
        }
      }
      if (counts == 0) break;
    }

  }

  return ret_count;
}

template<typename ... ParamTypes>
int Signal<ParamTypes...>::Disconnect(int start_pos, int counts) {
  internal::Token *it = nullptr;
  internal::Token *tmp = nullptr;
  int ret_count = 0;

  if (start_pos >= 0) {
    it = first_token_;
    while (it && (start_pos > 0)) {
      it = it->next;
      start_pos--;
    }

    while (it) {
      tmp = it;
      it = it->next;

      ret_count++;
      counts--;
      delete tmp;

      if (counts == 0) break;
    }

  } else {
    it = last_token_;
    while (it && (start_pos < -1)) {
      it = it->previous;
      start_pos++;
    }

    while (it) {
      tmp = it;
      it = it->previous;

      ret_count++;
      counts--;
      delete tmp;

      if (counts == 0) break;
    }

  }

  return ret_count;
}

template<typename ... ParamTypes>
template<typename T>
bool Signal<ParamTypes...>::IsConnectedTo(T *obj, void (T::*method)(ParamTypes..., SLOT)) const {
  internal::DelegateToken<ParamTypes..., SLOT> *delegate_token = nullptr;

  for (internal::Token *it = first_token_; it; it = it->next) {
    if (it->binding->trackable == obj) {
      delegate_token = dynamic_cast<internal::DelegateToken<ParamTypes..., SLOT> * > (it);
      if (delegate_token && (delegate_token->delegate().template Equal<T>(obj, method))) {
        return true;
      }
    }
  }
  return false;
}

template<typename ... ParamTypes>
bool Signal<ParamTypes...>::IsConnectedTo(const Signal<ParamTypes...> &other) const {
  internal::SignalToken<ParamTypes...> *signal_token = nullptr;

  for (internal::Token *it = first_token_; it; it = it->next) {
    if (it->binding->trackable == (&other)) {
      signal_token = dynamic_cast<internal::SignalToken<ParamTypes...> * > (it);
      if (signal_token && (signal_token->signal() == (&other))) {
        return true;
      }
    }
  }
  return false;
}

template<typename ... ParamTypes>
bool Signal<ParamTypes...>::IsConnectedTo(const Trackable *obj) const {
  internal::Token *token = first_token();
  internal::Binding *binding = obj->first_binding_;

  while (token && binding) {

    if (token->binding->trackable == obj) return true;
    if (binding->token->trackable == this) return true;

    token = token->next;
    binding = binding->next;
  }

  return false;
}

template<typename ... ParamTypes>
template<typename T>
int Signal<ParamTypes...>::CountConnections(T *obj, void (T::*method)(ParamTypes..., SLOT)) const {
  int count = 0;
  internal::DelegateToken<ParamTypes..., SLOT> *delegate_token = nullptr;

  for (internal::Token *it = first_token_; it; it = it->next) {
    if (it->binding->trackable == obj) {
      delegate_token = dynamic_cast<internal::DelegateToken<ParamTypes..., SLOT> * > (it);
      if (delegate_token && (delegate_token->delegate().template Equal<T>(obj, method))) {
        count++;
      }
    }
  }
  return count;
}

template<typename ... ParamTypes>
int Signal<ParamTypes...>::CountConnections(const Signal<ParamTypes...> &other) const {
  int count = 0;
  internal::SignalToken<ParamTypes...> *signal_token = nullptr;

  for (internal::Token *it = first_token_; it; it = it->next) {
    if (it->binding->trackable == (&other)) {
      signal_token = dynamic_cast<internal::SignalToken<ParamTypes...> * > (it);
      if (signal_token && (signal_token->signal() == (&other))) {
        count++;
      }
    }
  }
  return count;
}

template<typename ... ParamTypes>
int Signal<ParamTypes...>::CountConnections() const {
  int count = 0;
  for (internal::Token *it = first_token_; it; it = it->next) {
    count++;
  }
  return count;
}

template<typename ... ParamTypes>
void Signal<ParamTypes...>::Emit(ParamTypes ... Args) {
  Slot slot(first_token());

  while (nullptr != slot.token_) {
    slot.token_->slot_mark_head.push_back(&slot.mark_);
    static_cast<internal::CallableToken<ParamTypes..., SLOT> * > (slot.token_)->Invoke(Args..., &slot);

    if (slot.skip_) {
      slot.skip_ = false;
    } else {
      ++slot;
    }
  }
}

template<typename ... ParamTypes>
void Signal<ParamTypes...>::AuditDestroyingToken(internal::Token *token) {
  if (token == first_token_) first_token_ = token->next;
  if (token == last_token_) last_token_ = token->previous;
}

template<typename ... ParamTypes>
void Signal<ParamTypes...>::PushBackToken(internal::Token *token) {
#ifdef __DEBUG__
  assert(nullptr == token->trackable);
#endif

  if (last_token_) {
    last_token_->next = token;
    token->previous = last_token_;
  } else {
#ifdef __DEBUG__
    assert(nullptr == first_token_);
#endif
    token->previous = nullptr;
    first_token_ = token;
  }
  last_token_ = token;
  token->next = nullptr;
  token->trackable = this;
}

template<typename ... ParamTypes>
void Signal<ParamTypes...>::PushFrontToken(internal::Token *token) {
#ifdef __DEBUG__
  assert(nullptr == token->trackable);
#endif

  if (first_token_) {
    first_token_->previous = token;
    token->next = first_token_;
  } else {
#ifdef __DEBUG__
    assert(nullptr == last_token_);
#endif
    token->next = nullptr;
    last_token_ = token;
  }
  first_token_ = token;

  token->previous = nullptr;
  token->trackable = this;
}

template<typename ... ParamTypes>
void Signal<ParamTypes...>::InsertToken(int index, internal::Token *token) {
#ifdef __DEBUG__
  assert(nullptr == token->trackable);
#endif

  if (nullptr == first_token_) {
#ifdef __DEBUG__
    assert(nullptr == last_token_);
#endif
    token->next = nullptr;
    last_token_ = token;
    first_token_ = token;
    token->previous = nullptr;
  } else {
    if (index >= 0) {

      internal::Token *p = first_token_;
#ifdef __DEBUG__
      assert(p != nullptr);
#endif

      while (p && (index > 0)) {
        p = p->next;
        index--;
      }

      if (p) {  // insert before p

        token->previous = p->previous;
        token->next = p;

        if (p->previous) p->previous->next = token;
        else first_token_ = token;

        p->previous = token;

      } else {  // push back

        last_token_->next = token;
        token->previous = last_token_;
        last_token_ = token;
        token->next = nullptr;

      }

    } else {

      internal::Token *p = last_token_;
#ifdef __DEBUG__
      assert(p != nullptr);
#endif

      while (p && (index < -1)) {
        p = p->previous;
        index++;
      }

      if (p) {  // insert after p

        token->next = p->next;
        token->previous = p;

        if (p->next) p->next->previous = token;
        else last_token_ = token;

        p->next = token;

      } else {  // push front

        first_token_->previous = token;
        token->next = first_token_;
        first_token_ = token;
        token->previous = nullptr;

      }

    }
  }
  token->trackable = this;
}

template<typename ... ParamTypes>
void Signal<ParamTypes...>::DisconnectAll() {
  internal::Token *it = first_token_;
  internal::Token *tmp = nullptr;

  while (it) {
    tmp = it;
    it = it->next;
    delete tmp;
  }
}

/**
 * @ingroup base
 * @brief A reference to a corresponding signal
 */
template<typename ... ParamTypes>
class SignalRef {

 public:

  SignalRef() = delete;
  SignalRef &operator=(const SignalRef &) = delete;
  SignalRef(const SignalRef &) = delete;
  SignalRef &operator=(SignalRef &&) = delete;

  SignalRef(Signal<ParamTypes...> &signal)
      : signal_(&signal) {}

  SignalRef(SignalRef &&orig) noexcept
      : signal_(orig.signal_) {
    orig.signal_ = nullptr;
  }

  ~SignalRef() = default;

  template<typename T>
  void Connect(T *obj, void (T::*method)(ParamTypes..., SLOT), int index = -1) {
    signal_->Connect(obj, method, index);
  }

  void Connect(Signal<ParamTypes...> &signal, int index = -1) {
    signal_->Connect(signal, index);
  }

  template<typename T>
  void DisconnectAll(T *obj, void (T::*method)(ParamTypes..., SLOT)) {
    signal_->DisconnectAll(obj, method);
  }

  void DisconnectAll(Signal<ParamTypes...> &signal) {
    signal_->DisconnectAll(signal);
  }

  template<typename T>
  int Disconnect(T *obj, void (T::*method)(ParamTypes..., SLOT), int start_pos = -1, int counts = 1) {
    return signal_->Disconnect(obj, method, start_pos, counts);
  }

  int Disconnect(Signal<ParamTypes...> &signal, int start_pos = -1, int counts = 1) {
    return signal_->Disconnect(signal, start_pos, counts);
  }

  int Disconnect(int start_pos = -1, int counts = 1) {
    return signal_->Disconnect(start_pos, counts);
  }

  void DisconnectAll() {
    signal_->DisconnectAll();
  }

  template<typename T>
  bool IsConnectedTo(T *obj, void (T::*method)(ParamTypes..., SLOT)) const {
    return signal_->IsConnectedTo(obj, method);
  }

  bool IsConnectedTo(const Signal<ParamTypes...> &signal) const {
    return signal_->IsConnectedTo(signal);
  }

  bool IsConnectedTo(const Trackable *obj) const {
    return signal_->IsConnectedTo(obj);
  }

  template<typename T>
  int CountConnections(T *obj, void (T::*method)(ParamTypes..., SLOT)) const {
    return signal_->CountConnections(obj, method);
  }

  int CountConnections(const Signal<ParamTypes...> &signal) const {
    return signal_->CountConnections(signal);
  }

  int CountConnections() const {
    return signal_->CountConnections();
  }

  int CountBindings() const {
    return signal_->CountSignalBindings();
  }

 private:

  Signal<ParamTypes...> *signal_;

};

} // namespace base
} // namespace wiztk

#endif  // WIZTK_BASE_SIGCXX_HPP_
