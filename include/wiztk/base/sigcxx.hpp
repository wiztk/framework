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

/**
 * @file sigcxx.hpp
 * @brief Header file for Trackable and Signal classes.
 */

#ifndef WIZTK_BASE_SIGCXX_HPP_
#define WIZTK_BASE_SIGCXX_HPP_

#include "wiztk/base/delegate.hpp"
#include "wiztk/base/binode.hpp"

#include <cstddef>

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

namespace internal {

// Foward declarations:
struct TokenNode;

template<typename ... ParamTypes>
class SignalTokenNode;

/**
 * @ingroup base_intern
 * @brief A bidirectional node used to save the status of a Slot object.
 */
class WIZTK_NO_EXPORT SlotNode : public Binode<SlotNode> {
  friend class Slot;
 public:
  WIZTK_DECLARE_NONCOPYABLE(SlotNode);
  SlotNode() = default;
  ~SlotNode() override = default;
  SlotNode(SlotNode &&) = default;
  SlotNode &operator=(SlotNode &&) = default;
};

/**
 * @ingroup base_intern
 * @brief Base class of a bidirectional node used in Trackable or Signal only.
 */
class WIZTK_NO_EXPORT InterRelatedNodeBase : public Binode<InterRelatedNodeBase> {
  friend class Trackable;
  template<typename ... ParamTypes> friend
  class Signal;
};

/**
 * @ingroup base_intern
 * @brief The end point used in Trackable or Signal for building a double-ended list.
 */
class WIZTK_NO_EXPORT InterRelatedNodeEndpoint : public InterRelatedNodeBase {
  friend class Trackable;
  template<typename ... ParamTypes> friend
  class Signal;
};

/**
 * @ingroup base_intern
 * @brief A bi-node stored in Trackable with connection to a TokenNode.
 */
struct WIZTK_NO_EXPORT BindingNode : public InterRelatedNodeBase {
  BindingNode() = default;
  ~BindingNode() final;
  Trackable *trackable = nullptr;
  TokenNode *token = nullptr;
};

/**
 * @ingroup base_intern
 * @brief A bi-node stored in Signal with connection to a BindingNode.
 */
struct WIZTK_NO_EXPORT TokenNode : public InterRelatedNodeBase {
  friend class Slot;
  TokenNode() = default;
  ~TokenNode() override;
  Trackable *trackable = nullptr;
  BindingNode *binding = nullptr;
  SlotNode slot_mark_head;
};

/**
 * @ingroup base_intern
 * @brief A TokenNode with a virtual method to be invoked.
 * @tparam ParamTypes
 */
template<typename ... ParamTypes>
class WIZTK_NO_EXPORT AbstractInvokableTokenNode : public TokenNode {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(AbstractInvokableTokenNode);

  AbstractInvokableTokenNode() = default;

  ~AbstractInvokableTokenNode() override = default;

  virtual void Invoke(ParamTypes ... Args) = 0;

};

/**
 * @ingroup base_intern
 * @brief A TokenNode with a delegate.
 * @tparam ParamTypes
 */
template<typename ... ParamTypes>
class WIZTK_NO_EXPORT DelegateTokenNode : public AbstractInvokableTokenNode<ParamTypes...> {

 public:

  typedef Delegate<void(ParamTypes...)> DelegateType;

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(DelegateTokenNode);
  DelegateTokenNode() = delete;

  explicit DelegateTokenNode(const DelegateType &d)
      : AbstractInvokableTokenNode<ParamTypes...>(), delegate_(d) {}

  ~DelegateTokenNode() final = default;

  void Invoke(ParamTypes... Args) final {
    delegate_(Args...);
  }

  inline const DelegateType &delegate() const {
    return delegate_;
  }

 private:

  DelegateType delegate_;

};

/**
 * @ingroup base_intern
 * @brief A TokenNode points to a Signal.
 * @tparam ParamTypes
 */
template<typename ... ParamTypes>
class WIZTK_NO_EXPORT SignalTokenNode : public AbstractInvokableTokenNode<ParamTypes...> {

 public:

  typedef Signal<ParamTypes...> SignalType;

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(SignalTokenNode);
  SignalTokenNode() = delete;

  explicit SignalTokenNode(SignalType &signal)
      : AbstractInvokableTokenNode<ParamTypes...>(), signal_(&signal) {}

  ~SignalTokenNode() final = default;

  void Invoke(ParamTypes... Args) final {
    signal_->Emit(Args...);
  }

  const SignalType *signal() const {
    return signal_;
  }

 private:

  SignalType *signal_;

};

/**
 * @ingroup base_intern
 * @brief A simple double-ended queue to store bindings or tokens.
 * @tparam T Must be BindingNode or TokenNode
 */
template<typename T>
class WIZTK_NO_EXPORT InterRelatedDeque {

 public:

  /**
   * @brief Declare this class is non-copyable and non-movable.
   */
  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(InterRelatedDeque);

  /**
   * @brief Iterator.
   */
  class Iterator {

   public:

    Iterator() = delete;

    explicit Iterator(internal::InterRelatedNodeBase *node)
        : current_(node) {}

    ~Iterator() = default;

    Iterator &operator++() {
      current_ = current_->next();
      return *this;
    }

    Iterator &operator--() {
      current_ = current_->previous();
      return *this;
    }

    bool operator==(const Iterator &other) const { return current_ == other.current_; }
    bool operator!=(const Iterator &other) const { return current_ != other.current_; }

    T *get() const {
      return static_cast<T *>(current_);
    }

    T *operator->() const { return get(); }

    explicit operator bool() const {
      return nullptr == current_ ?
             false : (nullptr == current_->previous() ?
                      false : (nullptr != current_->next()));
    }

   private:

    internal::InterRelatedNodeBase *current_ = nullptr;

  };

  /**
   * @brief Const iterator.
   */
  class ConstIterator {

   public:

    ConstIterator() = delete;

    explicit ConstIterator(const internal::InterRelatedNodeBase *node)
        : current_(node) {}

    ~ConstIterator() = default;

    ConstIterator &operator++() {
      current_ = current_->next();
      return *this;
    }

    ConstIterator &operator--() {
      current_ = current_->previous();
      return *this;
    }

    bool operator==(const ConstIterator &other) const { return current_ == other.current_; }
    bool operator!=(const ConstIterator &other) const { return current_ != other.current_; }

    const T *get() const {
      return static_cast<const internal::BindingNode *>(current_);
    }

    const T *operator->() const { return get(); }

    explicit operator bool() const {
      return nullptr == current_ ?
             false : (nullptr == current_->previous() ?
                      false : (nullptr != current_->next()));
    }

   private:

    const internal::InterRelatedNodeBase *current_ = nullptr;

  };

  /**
 * @brief Reverse iterator.
 */
  class ReverseIterator {

   public:

    ReverseIterator() = delete;

    explicit ReverseIterator(internal::InterRelatedNodeBase *node)
        : current_(node) {}

    ~ReverseIterator() = default;

    ReverseIterator &operator++() {
      current_ = current_->previous();
      return *this;
    }

    ReverseIterator &operator--() {
      current_ = current_->next();
      return *this;
    }

    bool operator==(const ReverseIterator &other) const { return current_ == other.current_; }
    bool operator!=(const ReverseIterator &other) const { return current_ != other.current_; }

    T *get() const {
      return static_cast<T *>(current_);
    }

    T *operator->() const { return get(); }

    explicit operator bool() const {
      return nullptr == current_ ?
             false : (nullptr == current_->previous() ?
                      false : (nullptr != current_->next()));
    }

   private:

    internal::InterRelatedNodeBase *current_ = nullptr;

  };

  /**
 * @brief Const reverse iterator.
 */
  class ConstReverseIterator {

   public:

    ConstReverseIterator() = delete;

    explicit ConstReverseIterator(const internal::InterRelatedNodeBase *node)
        : current_(node) {}

    ~ConstReverseIterator() = default;

    ConstReverseIterator &operator++() {
      current_ = current_->previous();
      return *this;
    }

    ConstReverseIterator &operator--() {
      current_ = current_->next();
      return *this;
    }

    bool operator==(const ConstReverseIterator &other) const { return current_ == other.current_; }
    bool operator!=(const ConstReverseIterator &other) const { return current_ != other.current_; }

    const T *get() const {
      return static_cast<const internal::BindingNode *>(current_);
    }

    const T *operator->() const { return get(); }

    explicit operator bool() const {
      return nullptr == current_ ?
             false : (nullptr == current_->previous() ?
                      false : (nullptr != current_->next()));
    }

   private:

    const internal::InterRelatedNodeBase *current_ = nullptr;

  };

  /**
   * @brief Default constructor.
   */
  InterRelatedDeque() { head_.push_back(&tail_); }

  /**
   * @brief Destructor.
   */
  ~InterRelatedDeque() = default;

  /**
   * @brief Add element at the end.
   * @param node
   */
  void push_back(T *node) {
    // link binding and token before calling this method:
    _ASSERT(nullptr != node->trackable);
    tail_.push_front(node);
  }

  /**
   * @brief Insert element at the beginning.
   * @param node
   */
  void push_front(T *node) {
    // link binding and token before calling this method:
    _ASSERT(nullptr != node->trackable);
    head_.push_back(node);
  }

  /**
   * @brief Insert element at the given position.
   * @param node
   * @param index
   */
  void insert(T *node, int index = 0) {
    // link binding and token before calling this method:
    _ASSERT(nullptr != node->trackable);
    if (index >= 0) {
      auto it = begin();
      while ((it != end()) && (index > 0)) {
        ++it;
        index--;
      }
      it->push_front(node);
    } else {
      auto it = rbegin();
      while ((it != rend()) && (index < -1)) {
        ++it;
        index++;
      }
      it->push_back(node);
    }
  }

  /**
   * @brief Return iterator to beginning.
   * @return
   */
  Iterator begin() const { return Iterator(head_.next()); }

  /**
   * @brief Return const iterator to beginning.
   * @return
   */
  ConstIterator cbegin() const { return ConstIterator(head_.next()); }

  /**
   * @brief Return iterator to end.
   * @return
   */
  Iterator end() const {
    const internal::InterRelatedNodeBase *p = &tail_;
    return Iterator(const_cast<internal::InterRelatedNodeBase *>(p));
  }

  /**
   * @brief Return const iterator to end.
   * @return
   */
  ConstIterator cend() const { return ConstIterator(&tail_); }

  /**
   * @brief Return reverse iterator to reverse beginning
   * @return
   */
  ReverseIterator rbegin() const { return ReverseIterator(tail_.previous()); }

  /**
   * @brief Return const reverse iterator to reverse beginning.
   * @return
   */
  ConstReverseIterator crbegin() const { return ConstReverseIterator(tail_.previous()); }

  /**
   * @brief Return reverse iterator to reverse end.
   * @return
   */
  ReverseIterator rend() const {
    const internal::InterRelatedNodeBase *p = &head_;
    return ReverseIterator(const_cast<internal::InterRelatedNodeBase *>(p));
  }

  /**
   * @brief Return const reverse iterator to reverse end.
   * @return
   */
  ConstReverseIterator crend() const { return ConstReverseIterator(&head_); }

 private:

  typedef InterRelatedNodeEndpoint EndpointType;

  EndpointType head_;
  EndpointType tail_;

};

} // namespace internal

/**
 * @ingroup base
 * @brief Iterator and signature to a slot method
 *
 * A Slot object is created and destroyed when a signal is being emitted.
 *
 * It has two main purposes:
 *   - Works as an iterator
 *   - The last parameter in a slot method
 *
 * A Signal holds a list of token to support multicast, when it's being
 * emitted, it create a simple Slot object and use it as an iterater and call
 * each delegate (@ref Delegate) to the slot method or another signal.
 */
class WIZTK_EXPORT Slot {

  friend struct internal::TokenNode;
  friend class Trackable;

  template<typename ... ParamTypes> friend
  class Signal;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Slot);
  Slot() = delete;

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

 public:

  /**
   * @brief Get the Signal object which is just calling this slot
   */
  template<typename ... ParamTypes>
  Signal<ParamTypes...> *signal() const {
    return dynamic_cast<Signal<ParamTypes...> *>(iterator_->trackable);
  }

  /**
   * @brief The trackable object in which the slot method is being called
   * @return The trackable object receiving signal
   */
  Trackable *binding_trackable() const {
    return iterator_->binding->trackable;
  }

 private:

  typedef internal::InterRelatedDeque<internal::TokenNode> DequeType;
  typedef internal::InterRelatedDeque<internal::TokenNode>::Iterator IteratorType;

  explicit Slot(DequeType *deque)
      : deque_(deque), iterator_(deque->begin()), mark_(this) {}

  ~Slot() = default;

  Slot &operator++() {
    if (ref_count_ > 0) {
      --ref_count_;
    } else {
      ++iterator_;
    }
    return *this;
  }

  Slot &operator--() {
    if (ref_count_ > 0) {
      --ref_count_;
    } else {
      --iterator_;
    }
    return *this;
  }

  DequeType *deque_ = nullptr;
  IteratorType iterator_;
  size_t ref_count_ = 0;
  Mark mark_;

};

/**
 * @ingroup base
 * @brief A typedef of a pointer to a Slot.
 */
typedef Slot *SLOT;

/**
 * @ingroup base
 * @brief The basic class for an object which can provide slot methods
 */
class WIZTK_EXPORT Trackable {

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
  Trackable(const Trackable &);

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
  size_t CountSignalBindings(void (T::*method)(ParamTypes...)) const;

  /**
   * @brief Count all connections
   */
  size_t CountSignalBindings() const;

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

 private:

  static inline void Link(internal::TokenNode *token, internal::BindingNode *binding) {
    _ASSERT((nullptr == token->binding) && (nullptr == binding->token));
    token->binding = binding;
    binding->token = token;
  }

  static inline void PushFrontBinding(Trackable *trackable,
                                      internal::BindingNode *binding) {
    _ASSERT(nullptr == binding->trackable);
    binding->trackable = trackable;
    trackable->bindings_.push_front(binding);
  }

  static inline void PushBackBinding(Trackable *trackable,
                                     internal::BindingNode *binding) {
    _ASSERT(nullptr == binding->trackable);
    binding->trackable = trackable;
    trackable->bindings_.push_back(binding);
  }

  static inline void InsertBinding(Trackable *trackable,
                                   internal::BindingNode *binding,
                                   int index = 0) {
    _ASSERT(nullptr == binding->trackable);
    binding->trackable = trackable;
    trackable->bindings_.insert(binding, index);
  }

  internal::InterRelatedDeque<internal::BindingNode> bindings_;

};

template<typename T, typename ... ParamTypes>
void Trackable::UnbindAllSignalsTo(void (T::*method)(ParamTypes...)) {
  internal::BindingNode *tmp = nullptr;
  internal::DelegateTokenNode<ParamTypes...> *delegate_token = nullptr;

  auto it = bindings_.rbegin();
  while (it != bindings_.rend()) {
    tmp = it.get();
    ++it;

    delegate_token = dynamic_cast<internal::DelegateTokenNode<ParamTypes...> * > (tmp->token);
    if (delegate_token && (delegate_token->delegate().template Equal<T>((T *) this, method))) {
      delete tmp;
    }
  }
}

template<typename T, typename ... ParamTypes>
size_t Trackable::CountSignalBindings(void (T::*method)(ParamTypes...)) const {
  size_t count = 0;
  internal::DelegateTokenNode<ParamTypes...> *delegate_token = nullptr;

  for (auto it = bindings_.cbegin(); it != bindings_.cend(); ++it) {
    delegate_token =
        dynamic_cast<internal::DelegateTokenNode<ParamTypes...> * > (it->token);
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
class WIZTK_EXPORT Signal : public Trackable {

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

 private:

  static inline void PushFrontToken(Signal *signal, internal::TokenNode *token) {
    _ASSERT(nullptr == token->trackable);
    token->trackable = signal;
    signal->tokens_.push_front(token);
  }

  static inline void PushBackToken(Signal *signal, internal::TokenNode *token) {
    _ASSERT(nullptr == token->trackable);
    token->trackable = signal;
    signal->tokens_.push_back(token);
  }

  static inline void InsertToken(Signal *signal, internal::TokenNode *token, int index = 0) {
    _ASSERT(nullptr == token->trackable);
    token->trackable = signal;
    signal->tokens_.insert(token, index);
  }

  internal::InterRelatedDeque<internal::TokenNode> tokens_;

};

// Signal implementation:

template<typename ... ParamTypes>
template<typename T>
void Signal<ParamTypes...>::Connect(T *obj, void (T::*method)(ParamTypes..., SLOT), int index) {
  Delegate<void(ParamTypes..., SLOT)> d =
      Delegate<void(ParamTypes..., SLOT)>::template FromMethod<T>(obj, method);
  auto *token = new internal::DelegateTokenNode<ParamTypes..., SLOT>(d);
  auto *binding = new internal::BindingNode;

  Link(token, binding);
  InsertToken(this, token, index);
  PushBackBinding(obj, binding);  // always push back binding, don't care about the position in observer
}

template<typename ... ParamTypes>
void Signal<ParamTypes...>::Connect(Signal<ParamTypes...> &other, int index) {
  auto *token = new internal::SignalTokenNode<ParamTypes...>(
      other);
  auto *binding = new internal::BindingNode;

  Link(token, binding);
  InsertToken(this, token, index);
  PushBackBinding(&other, binding);  // always push back binding, don't care about the position in observer
}

template<typename ... ParamTypes>
template<typename T>
void Signal<ParamTypes...>::DisconnectAll(T *obj, void (T::*method)(ParamTypes..., SLOT)) {
  internal::DelegateTokenNode<ParamTypes..., SLOT> *delegate_token = nullptr;
  internal::TokenNode *tmp = nullptr;

  auto it = tokens_.rbegin();
  while (it != tokens_.rend()) {
    tmp = it.get();
    ++it;

    if (tmp->binding->trackable == obj) {
      delegate_token = dynamic_cast<internal::DelegateTokenNode<ParamTypes..., SLOT> * > (tmp);
      if (delegate_token && (delegate_token->delegate().template Equal<T>(obj, method))) {
        delete tmp;
      }
    }
  }
}

template<typename ... ParamTypes>
void Signal<ParamTypes...>::DisconnectAll(Signal<ParamTypes...> &other) {
  internal::SignalTokenNode<ParamTypes...> *signal_token = nullptr;
  internal::TokenNode *tmp = nullptr;

  auto it = tokens_.rbegin();
  while (it != tokens_.rend()) {
    tmp = it.get();
    ++it;

    if (tmp->binding->trackable == (&other)) {
      signal_token = dynamic_cast<internal::SignalTokenNode<ParamTypes...> * > (tmp);
      if (signal_token && (signal_token->signal() == (&other))) {
        delete tmp;
      }
    }
  }
}

template<typename ... ParamTypes>
template<typename T>
int Signal<ParamTypes...>::Disconnect(T *obj, void (T::*method)(ParamTypes..., SLOT), int start_pos, int counts) {
  internal::DelegateTokenNode<ParamTypes..., SLOT> *delegate_token = nullptr;
  internal::TokenNode *tmp = nullptr;
  int ret_count = 0;

  if (start_pos >= 0) {
    auto it = tokens_.begin();
    while ((it != tokens_.end()) && (start_pos > 0)) {
      ++it;
      start_pos--;
    }

    while (it != tokens_.end()) {
      tmp = it.get();
      ++it;

      if (tmp->binding->trackable == obj) {
        delegate_token = dynamic_cast<internal::DelegateTokenNode<ParamTypes..., SLOT> * > (tmp);
        if (delegate_token && (delegate_token->delegate().template Equal<T>(obj, method))) {
          ret_count++;
          counts--;
          delete tmp;
        }
      }
      if (counts == 0) break;
    }
  } else {
    auto it = tokens_.rbegin();
    while ((it != tokens_.rend()) && (start_pos < -1)) {
      ++it;
      start_pos++;
    }

    while (it != tokens_.rend()) {
      tmp = it.get();
      ++it;

      if (tmp->binding->trackable == obj) {
        delegate_token = dynamic_cast<internal::DelegateTokenNode<ParamTypes..., SLOT> * > (tmp);
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
  internal::SignalTokenNode<ParamTypes...> *signal_token = nullptr;
  internal::TokenNode *tmp = nullptr;
  int ret_count = 0;

  if (start_pos >= 0) {
    auto it = tokens_.begin();
    while ((it != tokens_.end()) && (start_pos > 0)) {
      ++it;
      start_pos--;
    }

    while (it != tokens_.end()) {
      tmp = it.get();
      ++it;

      if (tmp->binding->trackable == (&other)) {
        signal_token = dynamic_cast<internal::SignalTokenNode<ParamTypes...> * > (tmp);
        if (signal_token && (signal_token->signal() == (&other))) {
          ret_count++;
          counts--;
          delete tmp;
        }
      }
      if (counts == 0) break;
    }

  } else {
    auto it = tokens_.rbegin();
    while ((it != tokens_.rend()) && (start_pos < -1)) {
      ++it;
      start_pos++;
    }

    while (it != tokens_.rend()) {
      tmp = it.get();
      ++it;

      if (tmp->binding->trackable == (&other)) {
        signal_token = dynamic_cast<internal::SignalTokenNode<ParamTypes...> * > (tmp);
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
  internal::TokenNode *tmp = nullptr;
  int ret_count = 0;

  if (start_pos >= 0) {
    auto it = tokens_.begin();
    while ((it != tokens_.end()) && (start_pos > 0)) {
      ++it;
      start_pos--;
    }

    while (it != tokens_.end()) {
      tmp = it.get();
      ++it;

      ret_count++;
      counts--;
      delete tmp;

      if (counts == 0) break;
    }

  } else {
    auto it = tokens_.rbegin();
    while ((it != tokens_.rend()) && (start_pos < -1)) {
      ++it;
      start_pos++;
    }

    while (it != tokens_.rend()) {
      tmp = it.get();
      ++it;

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
  internal::DelegateTokenNode<ParamTypes..., SLOT> *delegate_token = nullptr;

  for (auto it = tokens_.begin(); it != tokens_.end();
       ++it) {
    if (it->binding->trackable == obj) {
      delegate_token = dynamic_cast<internal::DelegateTokenNode<ParamTypes..., SLOT> * > (it.get());
      if (delegate_token && (delegate_token->delegate().template Equal<T>(obj, method))) {
        return true;
      }
    }
  }
  return false;
}

template<typename ... ParamTypes>
bool Signal<ParamTypes...>::IsConnectedTo(const Signal<ParamTypes...> &other) const {
  internal::SignalTokenNode<ParamTypes...> *signal_token = nullptr;

  for (auto it = tokens_.begin(); it != tokens_.end();
       ++it) {
    if (it->binding->trackable == (&other)) {
      signal_token = dynamic_cast<internal::SignalTokenNode<ParamTypes...> * > (it.get());
      if (signal_token && (signal_token->signal() == (&other))) {
        return true;
      }
    }
  }
  return false;
}

template<typename ... ParamTypes>
bool Signal<ParamTypes...>::IsConnectedTo(const Trackable *obj) const {
  auto it = tokens_.begin();
  auto binding = obj->bindings_.begin();

  while ((it != tokens_.end()) && (binding != obj->bindings_.end())) {

    if (it->binding->trackable == obj) return true;
    if (binding.get()->token->trackable == this) return true;

    ++it;
    ++binding;
  }

  return false;
}

template<typename ... ParamTypes>
template<typename T>
int Signal<ParamTypes...>::CountConnections(T *obj, void (T::*method)(ParamTypes..., SLOT)) const {
  typedef internal::DelegateTokenNode<ParamTypes..., SLOT> DelegateTokenType;

  int count = 0;
  DelegateTokenType *delegate_token = nullptr;

  for (auto it = tokens_.begin(); it != tokens_.end();
       ++it) {
    if (it->binding->trackable == obj) {
      delegate_token = dynamic_cast<DelegateTokenType * > (it.get());
      if (delegate_token && (delegate_token->delegate().template Equal<T>(obj, method))) {
        count++;
      }
    }
  }
  return count;
}

template<typename ... ParamTypes>
int Signal<ParamTypes...>::CountConnections(const Signal<ParamTypes...> &other) const {
  typedef internal::SignalTokenNode<ParamTypes...> SignalTokenType;

  int count = 0;
  SignalTokenType *signal_token = nullptr;

  for (auto it = tokens_.begin(); it != tokens_.end();
       ++it) {
    if (it->binding->trackable == (&other)) {
      signal_token = dynamic_cast<SignalTokenType * > (it.get());
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
  for (auto it = tokens_.cbegin();
       it != tokens_.cend();
       ++it) {
    count++;
  }
  return count;
}

template<typename ... ParamTypes>
void Signal<ParamTypes...>::Emit(ParamTypes ... Args) {
  Slot slot(&tokens_);

  while (slot.iterator_) {
    slot.iterator_->slot_mark_head.push_back(&slot.mark_);
    static_cast<internal::AbstractInvokableTokenNode<ParamTypes..., SLOT> * > (slot.iterator_.get())->Invoke(Args...,
                                                                                                             &slot);
    ++slot;
  }
}

template<typename ... ParamTypes>
void Signal<ParamTypes...>::DisconnectAll() {
  internal::InterRelatedNodeBase *tmp = nullptr;

  auto it = tokens_.begin();
  while (it != tokens_.end()) {
    tmp = it.get();
    ++it;
    delete tmp;
  }
}

/**
 * @ingroup base
 * @brief A reference to a corresponding signal
 */
template<typename ... ParamTypes>
class WIZTK_EXPORT SignalRef {

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

  size_t CountBindings() const {
    return signal_->CountSignalBindings();
  }

 private:

  Signal<ParamTypes...> *signal_;

};

} // namespace base
} // namespace wiztk

#endif  // WIZTK_BASE_SIGCXX_HPP_
