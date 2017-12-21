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

#ifndef WIZTK_GUI_ABSTRACT_SHELL_VIEW_HPP_
#define WIZTK_GUI_ABSTRACT_SHELL_VIEW_HPP_

#include "abstract-event-handler.hpp"

#include "wiztk/base/rect.hpp"
#include "wiztk/base/margin.hpp"

#include "wiztk/gui/task-node.hpp"

#include <cstdint>
#include <string>

namespace wiztk {
namespace gui {

class Context;

/**
 * @ingroup gui
 * @brief Abstract class for shell views
 *
 * This is the base class for windows/menus/dialogs etc to show a shell surface
 * and managed by compositor.
 *
 * A sub class of an AbstractShellView usually should implement 4 virtual methods:
 *  - OnShown()
 *  - OnConfigureSize()
 *  - OnSaveSize()
 *  - RenderSurface()
 */
WIZTK_EXPORT class AbstractShellView : public AbstractEventHandler {

  friend class AbstractView;
  friend class ViewSurface;

 public:

  WIZTK_DECLARE_NONCOPYABLE(AbstractShellView);
  AbstractShellView() = delete;

  using Size   = base::SizeI;           /**< @brief Alias of base::SizeI */
  using Rect   = base::RectI;           /**< @brief Alias of base::RectI */
  using Margin = base::Margin;          /**< @brief Alias of base::Margin */

  /**
   * @brief A nested class to update the size
   */
  class GeometryTask : public TaskNode {

   public:

    WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(GeometryTask);
    GeometryTask() = delete;

    explicit GeometryTask(AbstractShellView *shell_view)
        : TaskNode(), shell_view_(shell_view) {}

    ~GeometryTask() final = default;

    /**
     * @brief Do the task to resize the shell view
     *
     * This method will call AbstractShellView::OnSaveSize(), record the last size and
     * use the xdg shell api to set the window geometry.
     */
    void Run() const final;

   private:

    AbstractShellView *shell_view_;

  };

  /**
   * @brief Enumeration values to indicate where the mouse
   * cursor/title view/client view is located
   *
   * TODO: use a diagram
   */
  enum Location {
    kInterior = 0,  /**< Inside the window frame including the title bar */
    kResizeTop = 1, /**< The top edge for resizing with mouse */
    kResizeBottom = 2,  /**< The bottom edge for resizing with mouse */
    kResizeLeft = 4,  /**< The left edge for resizing with mouse */
    kResizeTopLeft = 5, /**< The top-left corner for resizing with mouse */
    kResizeBottomLeft = 6,  /**< The bottom-left corner for resizing with mouse */
    kResizeRight = 8, /**< The right edge for resizing with mouse */
    kResizeTopRight = 9,  /**< The top-right corner for resizing with mouse */
    kResizeBottomRight = 10,  /**< The bottom-right corner for resizing with mouse */
    kResizeMask = 15, /**< The bitwise mask for the resizing flags above */
    kExterior = 16, /**< Outsize the window frame */
    kTitleBar = 17, /**< The title bar area */
    kClientArea = 18  /**< The client area */
  };

  /**
   * @brief Constructor
   * @param title A string of window title
   * @param parent Parent shell view
   * @param frame The frame used to show the background and title bar
   */
  explicit AbstractShellView(const char *title,
                             AbstractShellView *parent = nullptr);

  /**
   * @brief Constructor
   * @param width Width of this shell view
   * @param height Height of this shell view
   * @param title A string of window title
   * @param parent Parent shell view
   * @param frame The frame used to show the background and title bar
   */
  AbstractShellView(int width,
                    int height,
                    const char *title,
                    AbstractShellView *parent = nullptr);

  /**
   * @brief Destructor
   *
   * This destructor will destroy the content view attached and the shell frame.
   */
  virtual ~AbstractShellView();

  /**
   * @brief Set the title of this shell view
   * @param title A string of the window title
   */
  void SetTitle(const char *title);

  /**
   * @brief Set the App ID
   * @param app_id
   */
  void SetAppId(const char *app_id);

  void Show();

  void Close(__SLOT__);

  void Minimize(__SLOT__);

  void ToggleMaximize(__SLOT__);

  void ToggleFullscreen(const Output *output, __SLOT__);

  const std::string &GetTitle() const;

  bool IsFullscreen() const;

  bool IsMaximized() const;

  bool IsMinimized() const;

  bool IsFocused() const;

  bool IsResizing() const;

  bool IsShown() const;

  int GetWidth() const;

  int GetHeight() const;

  AbstractShellView *GetParent() const;

  static const Margin kResizingMargin;

 protected:

  /**
   * @brief Attach a view to this shell view
   * @param view
   *
   * @note This method does not check if the given view is nullptr.
   */
  void AttachView(AbstractView *view);

  /**
   * @brief Detach the view from this shell view
   * @param view
   *
   * @note This method does not check if the given view is nullptr.
   */
  void DetachView(AbstractView *view);

  /**
   * @brief A virtual method called when this shell view is first shown
   *
   * @note This is called only once.
   */
  virtual void OnShown() = 0;

  /**
   * @brief Configure the size of this shell view
   * @param old_size
   * @param new_size
   * @return
   *    - true: accept the new size
   *    - false: drop the new size and maintain the old one
   */
  virtual void OnConfigureSize(const Size &old_size, const Size &new_size) = 0;

  virtual void OnSaveSize(const Size &old_size, const Size &new_size) = 0;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave() override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseDown(MouseEvent *event) override;

  virtual void OnMouseUp(MouseEvent *event) override;

  virtual void OnKeyDown(KeyEvent *event) override;

  virtual void OnKeyUp(KeyEvent *event) override;

  virtual void OnRequestSaveGeometry(AbstractView *view) override;

  virtual void OnRequestUpdateFrom(AbstractView *view) override;

  virtual void OnEnterOutput(const ViewSurface *surface, const Output *output) override;

  virtual void OnLeaveOutput(const ViewSurface *surface, const Output *output) override;

  virtual void OnMaximized(bool);

  virtual void OnFullscreen(bool);

  virtual void OnFocus(bool);

  virtual void OnViewAttached(AbstractView *view);

  virtual void OnViewDetached(AbstractView *view);

  /**
   * @brief Schedule resize this shell view
   * @param size The size this shell view should have
   * @return
   *    - true Schedule resize this shell view
   *    - false Cancel the resize
   *
   * This method will schedule the geometry task and do the resize in the main loop, or cancel
   * the geometry task if the size value is the same as the last saved one.
   *
   * The geometry task will call the OnSaveSize() later.
   */
  bool RequestSaveSize(const Size &size);

  void MoveWithMouse(MouseEvent *event) const;

  void ResizeWithMouse(MouseEvent *event, uint32_t edges) const;

  ViewSurface *GetShellSurface() const;

  void DispatchMouseEnterEvent(AbstractView *view, MouseEvent *event);

  void DispatchMouseLeaveEvent();

  void DispatchMouseDownEvent(MouseEvent *event);

  void DispatchMouseUpEvent(MouseEvent *event);

  void DropShadow(const Context &context);

  static void DispatchUpdate(AbstractView *view);

  static void Draw(AbstractView *view, const Context &context);

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_ABSTRACT_SHELL_VIEW_HPP_
