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

#ifndef WIZTK_GUI_ABSTRACT_BUTTON_HPP_
#define WIZTK_GUI_ABSTRACT_BUTTON_HPP_

#include "abstract-view.hpp"

namespace wiztk {

// Forward declaration
namespace graphic {
class Font;
}

namespace gui {

/**
 * @ingroup gui
 * @brief The abstract class for buttons
 */
class AbstractButton : public AbstractView {

 public:

  using Font = graphic::Font;

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(AbstractButton);

  AbstractButton();

  AbstractButton(int width, int height);

  explicit AbstractButton(const std::string &text);

  SignalRef<> clicked() { return clicked_; }

  bool IsSensitive() const;

  bool IsHovered() const;

  bool IsPressed() const;

  const Font &GetFont() const;

  void SetFont(const graphic::Font &font);

  const std::string &GetText() const;

  void SetText(const std::string &text);

 protected:

  virtual ~AbstractButton();

  void OnMouseEnter(MouseEvent *event) override;

  void OnMouseLeave() override;

  void OnMouseMove(MouseEvent *event) override;

  void OnMouseDown(MouseEvent *event) override;

  void OnMouseUp(MouseEvent *event) override;

  void OnKeyDown(KeyEvent *event) override;

  void OnKeyUp(KeyEvent *event) override;

  void OnConfigureGeometry(const RectF &old_geometry,
                           const RectF &new_geometry) override;

  void OnSaveGeometry(const RectF &old_geometry,
                      const RectF &new_geometry) override;

  void SetSensitive(bool sensitive);

 private:

  struct Private;

  std::unique_ptr<Private> p_;

  Signal<> clicked_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_ABSTRACT_BUTTON_HPP_
