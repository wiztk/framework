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

#ifndef WIZTK_GUI_SLIDER_HPP_
#define WIZTK_GUI_SLIDER_HPP_

#include "abstract-slider.hpp"
#include "wiztk/base/color.hpp"

namespace wiztk {
namespace gui {

/**
 * @ingroup gui
 * @brief Slider
 */
class Slider : public AbstractSlider<int> {

 public:

  using ColorF = base::ColorF;

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Slider);

  explicit Slider(Orientation orientation = kHorizontal);

 protected:

  virtual ~Slider();

  void OnConfigureGeometry(const RectF &old_geometry, const RectF &new_geometry) final;

  void OnSaveGeometry(const RectF &old_geometry, const RectF &new_geometry) final;

  void OnMouseEnter(MouseEvent *event) final;

  void OnMouseLeave() final;

  void OnMouseMove(MouseEvent *event) final;

  void OnMouseDown(MouseEvent *event) final;

  void OnMouseUp(MouseEvent *event) final;

  void OnKeyDown(KeyEvent *event) final;

  void OnKeyUp(KeyEvent *event) final;

  void OnSetValue(const int &value) final;

  void OnSetMinimum(const int &minimum) final;

  void OnSetMaximum(const int &maximum) final;

  void OnDraw(const Context &context) override;

 private:

  bool hover_;

  ColorF regular_;
  ColorF highlight_;
  ColorF active_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_SLIDER_BAR_HPP_
