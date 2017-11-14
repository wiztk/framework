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

#ifndef WIZTK_GUI_ABSTRACT_SLIDER_HPP_
#define WIZTK_GUI_ABSTRACT_SLIDER_HPP_

#include "abstract-view.hpp"
#include "../base/types.hpp"

#include <algorithm>

namespace wiztk {
namespace gui {

template<typename T>
class AbstractSlider : public AbstractView {

  AbstractSlider(const AbstractSlider &) = delete;
  AbstractSlider &operator=(const AbstractSlider &) = delete;

 public:

  AbstractSlider(Orientation orientation = kHorizontal);

  virtual ~AbstractSlider();

  void SetValue(const T &value);

  void SetRange(const T &value1, const T &value2);

  void SetMinimum(const T &minimum);

  void SetMaximum(const T &maximum);

  void SetOrientation(Orientation orientation);

  Orientation orientation() const {
    return orientation_;
  }

  const T &value() const {
    return value_;
  }

  const T &minimum() const {
    return minimum_;
  }

  const T &maximum() const {
    return maximum_;
  }

  const T &step() const {
    return step_;
  }

  base::SignalRefT<const T &> changed() {
    return changed_;
  }

 protected:

  virtual void OnSetValue(const T &value) = 0;

  virtual void OnSetMinimum(const T &minimum) = 0;

  virtual void OnSetMaximum(const T &maximum) = 0;

  void EmitSignal() {
    changed_.Emit(value_);
  }

  void set_value(const T &value) {
    value_ = value;
  }

  void set_minimum(const T &minimum) {
    minimum_ = minimum;
  }

  void set_maximum(const T &maximum) {
    maximum_ = maximum;
  }

 private:

  Orientation orientation_;
  T value_;
  T minimum_;
  T maximum_;
  T step_;

  base::SignalT<const T &> changed_;

};

template<typename T>
AbstractSlider<T>::AbstractSlider(Orientation orientation)
    : AbstractView(),
      orientation_(orientation),
      value_(T(0)),
      minimum_(T(0)),
      maximum_(T(100)),
      step_(T(1)) {

}

template<typename T>
AbstractSlider<T>::~AbstractSlider() {

}

template<typename T>
void AbstractSlider<T>::SetValue(const T &value) {
  if (value_ == value) return;
  if (((value - step_) < minimum_) || ((value + step_) > maximum_)) return;

  OnSetValue(value);
}

template<typename T>
void AbstractSlider<T>::SetRange(const T &value1, const T &value2) {
  T minimum = std::min(value1, value2);
  T maximum = std::max(value1, value2);

  SetMinimum(minimum);
  SetMaximum(maximum);
}

template<typename T>
void AbstractSlider<T>::SetMinimum(const T &minimum) {
  if (minimum_ == minimum) return;
  if ((minimum + step_) > maximum_) return;

  OnSetMinimum(minimum);
}

template<typename T>
void AbstractSlider<T>::SetMaximum(const T &maximum) {
  if (maximum_ == maximum) return;
  if ((maximum - step_) < minimum_) return;

  OnSetMaximum(maximum);
}

template<typename T>
void AbstractSlider<T>::SetOrientation(Orientation orientation) {
  if (orientation_ == orientation) return;

  orientation_ = orientation;
  // TODO: resize in parent

  Update();
}

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_ABSTRACT_SLIDER_HPP_
