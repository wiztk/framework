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

#ifndef WIZTK_GUI_LABEL_HPP_
#define WIZTK_GUI_LABEL_HPP_

#include "abstract-view.hpp"

#include <wiztk/base/color.hpp>

namespace wiztk {

// Forward declarations
namespace graphics {
class Font;
}

namespace gui {

/**
 * @ingroup gui
 * @brief A label displays icon or text.
 */
WIZTK_EXPORT class Label : public AbstractView {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Label);

  /**
   * @brief Create a new Label object.
   * @tparam Args
   * @param args
   * @return
   */
  template<typename ... Args>
  static Label *Create(Args &&...args);

  void SetForeground(const base::ColorF &color);

  void SetBackground(const base::ColorF &color);

  void SetFont(const graphics::Font &font);

 protected:

  explicit Label(const std::string &text);

  Label(int width, int height, const std::string &text);

  ~Label() override;

  void OnConfigureGeometry(const RectF &old_geometry,
                           const RectF &new_geometry) override;

  void OnSaveGeometry(const RectF &old_geometry,
                      const RectF &new_geometry) final;

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
Label *Label::Create(Args &&... args) {
  return new Label(std::forward<Args>(args)...);
}

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_LABEL_HPP_
