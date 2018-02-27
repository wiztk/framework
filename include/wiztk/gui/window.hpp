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

#ifndef WIZTK_GUI_WINDOW_HPP_
#define WIZTK_GUI_WINDOW_HPP_

#include "wiztk/gui/abstract-shell-view.hpp"

namespace wiztk {
namespace gui {

/**
 * @ingroup gui
 * @brief A default window with a client-side decorations
 *
 * @example hello.cpp
 */
class WIZTK_EXPORT Window : public AbstractShellView {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Window);
  Window() = delete;

  /**
   * @brief Construct a 400 x 300 window with given title and flags
   * @param title
   * @param flags
   */
  explicit Window(const char *title);

  /**
   * @brief Construct a window with given size, title and flags
   * @param width
   * @param height
   * @param title
   * @param flags
   */
  Window(int width, int height, const char *title);

  ~Window() override;

  /**
   * @brief Get the title bar view
   * @return
   *    - A title bar object
   *    - nullptr if this is a frameless window
   */
  AbstractView *GetTitleBar() const;

  /**
   * @brief Get the content view
   * @return
   */
  AbstractView *GetContentView() const;

  /**
   * @brief Set the content view
   * @param view
   */
  void SetContentView(AbstractView *view);

  const Size &GetMinimalSize() const;

  const Size &GetPreferredSize() const;

  const Size &GetMaximalSize() const;

 protected:

  void OnShown() final;

  void OnRequestUpdateFrom(AbstractView *view) override;

  void OnConfigureSize(const Size &old_size, const Size &new_size) final;

  void OnSaveSize(const Size &old_size, const Size &new_size) final;

  void OnRenderSurface(ViewSurface *surface) final;

  void OnMouseEnter(MouseEvent *event) override;

  void OnMouseLeave() override;

  void OnMouseMove(MouseEvent *event) override;

  void OnMouseDown(MouseEvent *event) override;

  void OnMouseUp(MouseEvent *event) override;

  void OnKeyDown(KeyEvent *event) override;

  void OnFocus(bool) override;

  void OnViewAttached(AbstractView *view) final;

  void OnViewDetached(AbstractView *view) final;

  void OnEnterOutput(const ViewSurface *surface, const Output *output) final;

  void OnLeaveOutput(const ViewSurface *surface, const Output *output) final;

  int GetMouseLocation(const MouseEvent *event) const;

  Rect GetContentGeometry() const;

 private:

  void OnFullscreenButtonClicked(__SLOT__);

  struct Private;

  std::unique_ptr<Private> p_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_WINDOW_HPP_
