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

#ifndef WIZTK_GUI_INTERNAL_ABSTRACT_VIEW_ITERATORS_HPP_
#define WIZTK_GUI_INTERNAL_ABSTRACT_VIEW_ITERATORS_HPP_

#include <wiztk/gui/abstract-view.hpp>
#include "abstract-view_private.hpp"

namespace wiztk {
namespace gui {

/**
 * @ingroup gui_intern
 * @brief An iterator class to traverse in the view hierarchy
 */
WIZTK_NO_EXPORT class AbstractView::Iterator {

 public:

  Iterator(AbstractView *view = nullptr)
      : view_(view) {}

  Iterator(const Iterator &orig)
      : view_(orig.view_) {}

  ~Iterator() {}

  Iterator &operator=(const Iterator &other) {
    view_ = other.view_;
    return *this;
  }

  Iterator &operator=(AbstractView *view) {
    view_ = view;
    return *this;
  }

  /**
   * @brief Move to the next view
   * @return A reference to this iterator
   *
   * @note It's recommended to use this operator
   */
  Iterator &operator++() {
    view_ = view_->p_->next;
    return *this;
  }

  /**
   * @brief Move to the next view
   * @return A new iterator object
   */
  Iterator operator++(int) {
    Iterator it(view_->p_->next);
    return it;
  }

  /**
   * @brief Move to the previous view
   * @return A reference to this iterator
   *
   * @note It's recommended to use this operator
   */
  Iterator &operator--() {
    view_ = view_->p_->previous;
    return *this;
  }

  /**
   * @brief Move to the new previous view
   * @return A new iterator object
   */
  Iterator operator--(int) {
    Iterator it(view_->p_->previous);
    return it;
  }

  AbstractView *parent() const { return view_->p_->parent; }

  AbstractView *previous() const { return view_->p_->previous; }

  AbstractView *next() const { return view_->p_->next; }

  AbstractView *first_child() const { return view_->p_->first_child; }

  AbstractView *last_child() const { return view_->p_->last_child; }

  AbstractView *view() const { return view_; }

  operator bool() const { return nullptr != view_; }

 private:

  AbstractView *view_;

};

class AbstractView::ConstIterator {

 public:

  ConstIterator(const AbstractView *view = nullptr)
      : view_(view) {}

  ConstIterator(const ConstIterator &orig)
      : view_(orig.view_) {}

  ~ConstIterator() {}

  ConstIterator &operator=(const ConstIterator &other) {
    view_ = other.view_;
    return *this;
  }

  ConstIterator &operator=(AbstractView *view) {
    view_ = view;
    return *this;
  }

  /**
   * @brief Move to the next view
   * @return A reference to this iterator
   *
   * @note It's recommended to use this operator
   */
  ConstIterator &operator++() {
    view_ = view_->p_->next;
    return *this;
  }

  /**
   * @brief Move to the next view
   * @return A new iterator object
   */
  ConstIterator operator++(int) {
    ConstIterator it(view_->p_->next);
    return it;
  }

  /**
   * @brief Move to the previous view
   * @return A reference to this iterator
   *
   * @note It's recommended to use this operator
   */
  ConstIterator &operator--() {
    view_ = view_->p_->previous;
    return *this;
  }

  /**
   * @brief Move to the new previous view
   * @return A new iterator object
   */
  ConstIterator operator--(int) {
    ConstIterator it(view_->p_->previous);
    return it;
  }

  const AbstractView *parent() const { return view_->p_->parent; }

  const AbstractView *previous() const { return view_->p_->previous; }

  const AbstractView *next() const { return view_->p_->next; }

  const AbstractView *first_child() const { return view_->p_->first_child; }

  const AbstractView *last_child() const { return view_->p_->last_child; }

  const AbstractView *view() const { return view_; }

  operator bool() const { return nullptr != view_; }

 private:

  const AbstractView *view_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_INTERNAL_ABSTRACT_VIEW_ITERATORS_HPP_
