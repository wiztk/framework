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

#ifndef WIZTK_GUI_ABSTRACT_VIEW_HPP_
#define WIZTK_GUI_ABSTRACT_VIEW_HPP_

#include "abstract-event-handler.hpp"

#include "wiztk/base/types.hpp"
#include "wiztk/base/size.hpp"
#include "wiztk/base/rect.hpp"
#include "wiztk/base/padding.hpp"

#include "anchor-group.hpp"

#include <memory>

namespace wiztk {
namespace gui {

class AbstractShellView;
class AbstractLayout;
class Context;

/**
 * @ingroup gui
 * @brief Layout policy to indicate how a view is resized in a layout
 */
enum LayoutPolicy {
  kLayoutMinimal,                       /**< Recommend to use the minimal size */
  kLayoutPreferred,                     /**< Recommend to use the preferred size */
  kLayoutMaximal,                       /**< Recommend to use the maximal size */
  kLayoutFixed,                         /**< Recommend to use the current size and do not change */
  kLayoutExpandable                     /**< Recommend to expand to any value between minimal and maximal size */
};

/**
 * @ingroup gui
 * @brief An abstract base class for views
 *
 * AbstractView is an abstract base class for views. A view in SkLand is a
 * rectangle in an AbstractShellView object and handle events.  You typically
 * don't use this class directly. Instead, you use or create a subclass.
 *
 * A view can have parent and subviews, when you create a GUI application, it
 * generates a view hierarchy.
 *
 * As a sub class of AbstractEventHandler, a view object can have arbitrary
 * number of surfaces. Or it can share the surface with others which is managed
 * in one of parent views. But in most cases, it draws on the surface hold in a
 * shell view (e.g. Window).
 *
 * An AbstractView object SHOULD always be created by new operator and MUST be
 * destroyed by Destroy() method.
 *
 * @see AbstractShellView
 * @see Surface
 */
WIZTK_EXPORT class AbstractView : public AbstractEventHandler {

  friend class AbstractShellView;
  friend class AbstractLayout;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(AbstractView);

  using Size    = base::SizeI;          /**< @brief Alias of base::SizeI */
  using Rect    = base::RectI;          /**< @brief Alias of base::RectI */
  using RectF   = base::RectF;          /**< @brief Alias of base::RectF */
  using Padding = base::Padding;        /**< @brief Alias of base::Padding */

  enum GeometryDirtyFlagMask {
    kDirtyLeftMask = 0x1 << 0,
    kDirtyTopMask = 0x1 << 1,
    kDirtyRightMask = 0x1 << 2,
    kDirtyBottomMask = 0x1 << 3,
    kDirtyWidthMask = 0x1 << 4,
    kDirtyHeightMask = 0x1 << 5
  };

  class Iterator;
  class ConstIterator;

  class GeometryTask : public Task {

   public:

    WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(GeometryTask);
    GeometryTask() = delete;

    explicit GeometryTask(AbstractView *view)
        : Task(), view_(view) {}

    virtual ~GeometryTask() = default;

    virtual void Run() const final;

    static GeometryTask *Get(const AbstractView *view);

   private:

    AbstractView *view_;

  };

  class RenderNode : public base::BiNode {

   public:

    WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(RenderNode);
    RenderNode() = delete;

    explicit RenderNode(AbstractView *view)
        : base::BiNode(), view_(view) {}

    virtual ~RenderNode() = default;

    AbstractView *view() const { return view_; }

    static RenderNode *Get(const AbstractView *view);

   private:

    AbstractView *view_;

  };

  /**
   * @brief Default constructor
   *
   * This create a 400 x 300 view.
   */
  AbstractView();

  /**
   * @brief Create a view by given size
   */
  AbstractView(int width, int height);

  /**
   * @brief Set the minimal width
   * @param width
   *
   * If the width is larger than maximal width, nothing will happen.
   *
   * The preferred width will be reset to new minimal width if it's less.  This
   * method will ask for re-layout if this view is in a layout.
   */
  void SetMinimalWidth(int width);

  /**
   * @brief Set the minimal height
   * @param height
   *
   * If the height is larger than maximal height, nothing will happen.
   *
   * The preferred height will be reset to new minimal height less.  This method
   * will ask for re-layout if this view is in a layout.
   */
  void SetMinimalHeight(int height);

  /**
   * @brief Get the minimal width
   * @return
   */
  int GetMinimalWidth() const;

  /**
   * @brief Get the minimal height
   * @return
   */
  int GetMinimalHeight() const;

  /**
   * @brief Set the preferred width
   * @param width
   *
   * If width is less than minimal width or larger than maximal width, nothing
   * will happen.  This method will ask for re-layout if this view is in a
   * layout.
   */
  void SetPreferredWidth(int width);

  /**
   * @brief Set the preferred height
   * @param height
   *
   * If height is less than minimal height or larger than maximal height,
   * nothing will happen.  This method will ask for re-layout if this view is in
   * a layout.
   */
  void SetPreferredHeight(int height);

  /**
   * @brief Get the preferred width
   * @return
   */
  int GetPreferredWidth() const;

  /**
   * @brief Get the preferred height
   * @return
   */
  int GetPreferredHeight() const;

  /**
   * @brief Set the maximal width
   * @param width
   *
   * If width is less than the minimal width, nothing will happen.
   *
   * The preferred width will be reset to the new maximal width if it's larger.
   * This method will ask for re-layout if this view is in a layout.
   */
  void SetMaximalWidth(int width);

  /**
   * @brief Set the maximal height
   * @param height
   *
   * If height is less than the minimal height, nothing will happen.
   *
   * The preferred height will be reset to the new maximal height if it's
   * larger. This method will ask for re-layout if this view is in a layout.
   */
  void SetMaximalHeight(int height);

  /**
   * @brief Get the maximal width
   * @return
   */
  int GetMaximalWidth() const;

  /**
   * @brief Get the maximal height
   * @return
   */
  int GetMaximalHeight() const;

  /**
   * @brief Set the layout policy along X
   * @param policy
   *
   * This method will ask for re-layout if this view is in a layout.
   */
  void SetLayoutPolicyOnX(LayoutPolicy policy);

  /**
   * @brief Get the layout policy along X
   * @return
   */
  LayoutPolicy GetLayoutPolicyOnX() const;

  /**
   * @brief Set the layout policy along Y
   * @param policy
   *
   * This method will ask for re-layout if this view is in a layout.
   */
  void SetLayoutPolicyOnY(LayoutPolicy policy);

  /**
   * @brief Get the layout policy along Y
   * @return
   */
  LayoutPolicy GetLayoutPolicyOnY() const;

  const Padding &GetPadding() const;

  /**
   * @brief Move this view to the given position in window coordinate
   * @param x
   * @param y
   */
  void MoveTo(int x, int y);

  /**
   * @brief Resiz this view
   * @param width
   * @param height
   */
  void Resize(int width, int height);

  /**
   * @brief Get X position in window coordinate
   * @return
   */
  int GetX() const;

  /**
   * @brief Get Y position in window coordinate
   * @return
   */
  int GetY() const;

  /**
   * @brief Get left position relative to parent
   * @return
   */
  int GetLeft() const;

  /**
   * @brief Set the left position relative to parent
   * @param left
   */
  void SetLeft(int left);

  /**
   * @brief Get top position relative to parent
   * @return
   */
  int GetTop() const;

  /**
   * @brief Set top position relative to parent
   * @param top
   */
  void SetTop(int top);

  /**
   * @brief Get right position relative to parent
   * @return
   */
  int GetRight() const;

  /**
   * @brief Set right position relative to parent
   * @param right
   */
  void SetRight(int right);

  /**
   * @brief Get bottom position relative to parent
   * @return
   */
  int GetBottom() const;

  /**
   * @brief Set bottom position relative to parent
   * @param bottom
   */
  void SetBottom(int bottom);

  /**
   * @brief Get the width of the geometry
   * @return
   */
  int GetWidth() const;

  /**
   * @brief Get the height of the geometry
   * @return
   */
  int GetHeight() const;

  /**
   * @brief Get the center position of geometry along X axis
   * @return
   */
  float GetXCenter() const;

  /**
   * @brief Get the center position of geometry along Y axis
   * @return
   */
  float GetYCenter() const;

  /**
   * @brief Get the location and size of this view
   *
   * Geometry determines how a view interacts with input events.
   */
  const RectF &GetGeometry() const;

  /**
   * @brief Get the internal origin and size of this view
   *
   * Bounds determines how a view render on a surface. (e.g. it may drop shadow outside)
   */
  const RectF &GetBounds() const;

  void SetBounds(const RectF &bounds);

  void SetBounds(float left, float top, float right, float bottom);

  bool IsVisible() const;

  /**
   * @brief Add an anchor to target view for layout
   * @param target
   * @param align
   *   - If this view is the parent of target or vice versa, add anchors on the same edge
   *   - If this view and target view are siblings:
   *     - If kAlignLeft: put this view at the left side of target view (connect right anchor to left anchor)
   *     - If kAlignTop: put this view at the top side of target view
   *     - If kAlignRight: put this view at the right side of target view
   *     - If kAlignBottom: put this view at the bottom side of target view
   * @param distance
   *
   * @note This method does not check if there's already anchors connect these 2
   * views.
   */
  void AddAnchorTo(AbstractView *target, Alignment align, int distance);

  const AnchorGroup &GetAnchorGroup(Alignment align) const;

  /**
   * @brief Update the display of this widget
   * @param[in] validate
   *   - true Request update
   *   - false Cancel the update request
   */
  void Update(bool validate = true);

  /**
   * @brief Returns a boolean if this view contains the given pointer position
   * @param x
   * @param y
   * @return
   */
  virtual bool Contain(int x, int y) const;

  /**
   * @brief Destroy and delete this object
   *
   * This method will emit an 'destroyed' signal at first and call OnDestroy()
   * before doing some clean up work.
   */
  void Destroy();

  /**
   * @brief Get the parent view
   * @return
   */
  AbstractView *GetParent() const;

  /**
   * @brief Get the layout
   * @return
   */
  AbstractLayout *GetLayout() const;

  /**
   * @brief Get the shell view that contains this View object
   * @return
   */
  AbstractShellView *GetShellView() const;

  SignalRef<AbstractView *> destroyed() { return destroyed_; }

 protected:

  /**
   * @brief Destructor
   *
   * @note You should never use delete to destroy an AbstractView object. An
   * AbstractView should always be created by new operator and use the Destroy()
   * to destroy itself.
   */
  virtual ~AbstractView();

  /**
   * @brief Callback to draw this view on a canvas
   * @param context
   */
  virtual void OnDraw(const Context &context) = 0;

  /**
   * @brief Callback when a child view is added
   * @param view
   */
  virtual void OnChildAdded(AbstractView *view);

  /**
   * @brief Callback when a child view is removed
   * @param view
   */
  virtual void OnChildRemoved(AbstractView *view);

  /**
   * @brief Callback when this view is added to a parent
   */
  virtual void OnAddedToParent();

  /**
   * @brief Callback when this view is removed from a parent
   * @param original_parent
   */
  virtual void OnRemovedFromParent(AbstractView *original_parent);

  /**
   * @brief Callback when this view is attached to a shell view
   */
  virtual void OnAttachedToShellView();

  /**
   * @brief Callback when this view is detached to a shell view
   * @param shell_view
   */
  virtual void OnDetachedFromShellView(AbstractShellView *shell_view);

  /**
   * @brief A view request an update
   * @param view This view or a sub view in hierarchy
   */
  virtual void OnRequestUpdateFrom(AbstractView *view) override;

  virtual void OnRenderSurface(Surface *surface) override;

  /**
   * @brief Callback when this view enters an output
   * @param surface
   * @param output
   */
  virtual void OnEnterOutput(const Surface *surface, const Output *output) override;

  /**
   * @brief Callback when this view leaves an output
   * @param surface
   * @param output
   */
  virtual void OnLeaveOutput(const Surface *surface, const Output *output) override;

  /**
   * @brief Callback when any part of geometry will change
   * @param dirty_flag Bitwise flag of which part of geometry will change
   *   - 0: no change, this is possible if the geometry is changed back to last value by several methods
   *   - Bitwise flag:
   * @param old_geometry
   * @param new_geometry
   *
   * The new size will never smaller than minimal size or larger than maximal size.
   */
  virtual void OnConfigureGeometry(const RectF &old_geometry,
                                   const RectF &new_geometry) = 0;

  virtual void OnRequestSaveGeometry(AbstractView *view) override;

  /**
   * @brief Callback when the geometry changes before draw this view
   * @param dirty_flag Bitwise flag of which part of geometry changes
   * @param old_geometry
   * @param new_geometry
   *
   * This virtual method is called only when there's new geometry need to be saved before drawing this
   * view.
   */
  virtual void OnSaveGeometry(const RectF &old_geometry,
                              const RectF &new_geometry) = 0;

  /**
   * @brief Callback when destroyed
   *
   * By default this method does nothing, sub class can override this.
   */
  virtual void OnDestroy();

  /**
   * @brief Update this view and all sub views
   *
   * By default this method will update this view and all recursively update all
   * sub views.  Sub class can override this method to select part of sub views
   * to update.
   */
  virtual void DispatchUpdate();

  /**
   * @brief Dispatch mouse enter event down to the target view
   * @param event
   * @return
   *   - A sub view object
   *   - nullptr if no sub view need to handle the mouse enter event
   *
   * By default this method check all sub views and return the one contains the
   * cursor position. Sub class can override this.
   */
  virtual AbstractView *DispatchMouseEnterEvent(MouseEvent *event);

  /**
   * @brief Schedule change the geometry of this view
   * @param[in] validate
   *	- true: schedule change the geometry in task deque
   *	- false: cancel the geometry task
   */
  bool RequestSaveGeometry(const RectF &geometry);

  void TrackMouseMotion(MouseEvent *event);

  void UntrackMouseMotion();

  AbstractView *GetChildAt(int index) const;

  /**
   * @brief Push a child object to the front
   * @param child
   *
   * @warning This method does not check if child is nullptr
   */
  void PushFrontChild(AbstractView *child);

  /**
   * @brief Insert a child object at the given index
   * @param child
   * @param index The position to be inserted before, default is 0, same as push front
   *
   * @warning This method does not check if child is nullptr
   */
  void InsertChild(AbstractView *child, int index = 0);

  /**
   * @brief Push a child object to the back
   * @param child
   *
   * @warning This method does not check if child is nullptr
   */
  void PushBackChild(AbstractView *child);

  /**
   * @brief Remove a child object from the children list
   * @param child
   * @return nullptr if the object is not a child, or the removed object
   *
   * @warning This method does not check if the param is nullptr
   */
  AbstractView *RemoveChild(AbstractView *child);

  /**
   * @brief Destroy all children
   */
  void ClearChildren();

  static bool SwapIndex(AbstractView *view1, AbstractView *view2);

  static bool InsertSiblingBefore(AbstractView *src, AbstractView *dst);

  static bool InsertSiblingAfter(AbstractView *src, AbstractView *dst);

  static void MoveToFirst(AbstractView *view);

  static void MoveToLast(AbstractView *view);

  static void MoveForward(AbstractView *view);

  static void MoveBackward(AbstractView *view);

 private:

  struct Private;

  std::unique_ptr<Private> p_;

  base::Signal<AbstractView *> destroyed_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_ABSTRACT_VIEW_HPP_
