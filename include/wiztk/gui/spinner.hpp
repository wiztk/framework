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

#ifndef WIZTK_GUI_SPINNER_HPP_
#define WIZTK_GUI_SPINNER_HPP_

#include "wiztk/gui/abstract-view.hpp"

namespace wiztk {
namespace gui {

/**
 * @ingroup gui
 * @brief Spinner
 */
class WIZTK_EXPORT Spinner : public AbstractView {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Spinner);

  template<typename ... Args>
  static Spinner *Create(Args &&... args);

 protected:

  Spinner();

  ~Spinner() override;

  void OnConfigureGeometry(const RectF &old_rect, const RectF &new_rect) override;

  void OnSaveGeometry(const RectF &old_rect, const RectF &new_rect) override;

  void OnMouseEnter(MouseEvent *event) override;

  void OnMouseLeave() override;

  void OnMouseMove(MouseEvent *event) override;

  void OnMouseDown(MouseEvent *event) override;

  void OnMouseUp(MouseEvent *event) override;

  void OnKeyDown(KeyEvent *event) override;

  void OnKeyUp(KeyEvent *event) override;

  void OnDraw(const Context &context) override;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

template<typename ... Args>
Spinner *Spinner::Create(Args &&... args) {
  return new Spinner(std::forward<Args>(args)...);
}

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_SPINNER_HPP_
