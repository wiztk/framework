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

#include "wiztk/gui/abstract-view.hpp"

#include "wiztk/base/color.hpp"

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
class WIZTK_EXPORT Label : public AbstractView {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Label);

  using ColorF = base::ColorF;
  using Font = graphics::Font;

 public:

  /**
   * @brief Initialize a new instance of the Label class.
   * @tparam Args Variadic template parameter.
   * @param args Variadic function parameter, see the constructors of this class:
   *     - Label(const std::string &)
   *     - Label(int, int, const std::string &)
   * @return A new pointer to a Label object.
   */
  template<typename ... Args>
  static Label *Create(Args &&...args);

  /**
   * @brief Set the foreground color of this label.
   * @param color
   */
  void SetForeColor(const ColorF &color);

  /**
   * @brief Get the foreground color of this label.
   * @return
   */
  const ColorF &GetForeColor() const;

  /**
   * @brief Set the background color of this label.
   * @param color
   */
  void SetBackColor(const ColorF &color);

  /**
   * @brief Get the background color of this label.
   * @return
   */
  const ColorF &GetBackColor() const;

  /**
   * @brief Set the font of the text displayed by this label.
   * @param font
   */
  void SetFont(const Font &font);

  /**
   * @brief Get the font of the text displayed by this label.
   * @return
   */
  const Font &GetFont() const;

 protected:

  /**
   * @brief Constructor
   * @param text
   */
  explicit Label(const std::string &text);

  /**
   * @brief Constructor
   * @param width Width of this view.
   * @param height Height of this view.
   * @param text The text to be shown.
   */
  Label(int width, int height, const std::string &text);

  /**
   * @brief Destructor.
   */
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
