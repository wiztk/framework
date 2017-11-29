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

#ifndef WIZTK_GUI_SURFACE_HPP_
#define WIZTK_GUI_SURFACE_HPP_

#include "wiztk/base/sigcxx.hpp"
#include "wiztk/base/margin.hpp"
#include "wiztk/base/point.hpp"

#include "wiztk/graphic/abstract-surface.hpp"

#include "abstract-view.hpp"
#include "task.hpp"

#include <wayland-egl.h>
#include <memory>

namespace wiztk {
namespace gui {

class Buffer;
class Output;
class InputEvent;
class Region;
class AbstractRenderingAPI;

/**
 * @ingroup gui
 * @brief Surface for views
 *
 * Surface represents a native wayland surface to display views.
 *
 * A surface can have parent or arbitrary sub surfaces and acts as different
 * roles.
 *
 * According to wayland protocol, a surface without a parent must be a shell
 * surface, a shell surface is created and managed in a AbstractShellView
 * object to display window, popup menu or tooltip etc.
 *
 * A surface which has a parent is called a sub surface, it's usually used in
 * special widgets (e.g. video widget, or EGL widget) as such widgets need their
 * own surface to display something.
 *
 * Each shell surface or sub surface can also be a EGL surface to display 3D
 * scene, otherwise, it displays 2D contents through wayland shared memory
 * buffer.
 *
 * All surfaces in an SkLand application are stacked by the order defined in
 * wayland protocol. [TODO: use an image to show the surface stack.]
 *
 * This class has several nested classes to identify and control the surface
 * role. You cannot create or delete a surface object directly. Instead, you
 * need to use the nested class. For example:
 *
 *   - Create a toplevel shell surface with Surface::Shell::Toplevel::Create()
 *   - Create a sub surface with Surface::Sub::Create()
 *
 * You can also use Surface::EGL::Get() to transform a 2D surface to 3D surface,
 * and delete the Surface::EGL object will transform back to 2D.
 */
class ViewSurface : public graphic::AbstractSurface, public base::Trackable {

  using Point  = base::Point2I;
  using Margin = base::Margin;

  friend class Application;
  friend class Display;
  friend class Callback;
  friend class AbstractRenderingAPI;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(ViewSurface);
  ViewSurface() = delete;

  class Shell;
  class Sub;

  /**
   * @brief Transform enums
   */
  enum Transform {
    kTransformNormal = 0,           /**< WL_OUTPUT_TRANSFORM_NORMAL */
    kTransform90 = 1,           /**< WL_OUTPUT_TRANSFORM_90 */
    kTransform180 = 2,           /**< WL_OUTPUT_TRANSFORM_180 */
    kTransform270 = 3,           /**< WL_OUTPUT_TRANSFORM_270 */
    kTransformFlipped = 4,           /**< WL_OUTPUT_TRANSFORM_FLIPPED */
    kTransformFlipped90 = 5,           /**< WL_OUTPUT_TRANSFORM_FLIPPED_90 */
    kTransformFlipped180 = 6,           /**< WL_OUTPUT_TRANSFORM_FLIPPED_180 */
    kTransformFlipped270 = 7            /**< WL_OUTPUT_TRANSFORM_FLIPPED_270 */
  };

  /**
    * @brief Commit behaviour of the sub-surface
    */
  enum CommitMode {
    kSynchronized,                      /**< Synchronized mode */
    kDesynchronized                     /**< Desynchronized mode */
  };

  ~ViewSurface() override;

  static int GetShellSurfaceCount() { return kShellSurfaceCount; }

  /**
   * @brief Attach a shared memory buffer
   */
  void Attach(Buffer *buffer, int32_t x = 0, int32_t y = 0);

  /**
   * @brief Add this surface in the commit task list
   *
   * This method add this surface in the commit task list and will commit the
   * native wayland surface in the event loop.
   *
   * If this surface is a sub surface and commit behaviour is synchronized, this
   * method will commit the shell surface (main surface) too.
   */
  void Commit();

  void SetCommitMode(CommitMode mode);

  /**
   * @brief Mark the damaged region of the surface
   */
  void Damage(int surface_x, int surface_y, int width, int height);

  void SetInputRegion(const Region &region);

  void SetOpaqueRegion(const Region &region);

  void SetTransform(Transform transform);

  Transform GetTransform() const;

  void SetScale(int32_t scale);

  int32_t GetScale() const;

  void DamageBuffer(int32_t x, int32_t y, int32_t width, int32_t height);

  void Update(bool validate = true);

  /**
   * @brief Get defferred redraw task deque
   * @return
   */
  base::DequeT<AbstractView::RenderNode> &GetViewRenderDeque() const;

  ViewSurface *GetShellSurface();

  /**
   * @brief Get the position in window coordinates
   * @return Global position
   */
  Point GetWindowPosition() const;

  /**
   * @brief Get the parent surface
   * @return
   */
  ViewSurface *GetParent() const;

  /**
   * @brief The sibling surface above this one
   * @return
   */
  ViewSurface *GetSiblingAbove() const;

  /**
   * @brief The sibling surface below this one
   * @return
   */
  ViewSurface *GetSiblingBelow() const;

  /**
   * @brief The shell surface placed over this one
   * @return
   */
  ViewSurface *GetUpperShell() const;

  /**
   * @brief The shell surface placed under this one
   * @return
   */
  ViewSurface *GetLowerShell() const;

  AbstractEventHandler *GetEventHandler() const;

  /**
   * @brief Set the graphic library interface for this surface
   * @param api An allocated GLInterface object or nullptr to unset and
   * use shm back.
   *
   * @note The GLInterface will be deleted when the surface destroyed.
   */
  void SetRenderingAPI(AbstractRenderingAPI *api);

  AbstractRenderingAPI *GetRenderingAPI() const;

  const Point &GetRelativePosition() const;

  void Render(Delegate *delegate) override;

 private:

  class RenderTask : public Task {

   public:

    WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(RenderTask);
    RenderTask() = delete;

    explicit RenderTask(ViewSurface *surface)
        : surface_(surface) {}

    ~RenderTask() final = default;

    void Run() const final;

   private:

    ViewSurface *surface_;

  };

  class CommitTask : public Task {

   public:

    WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(CommitTask);
    CommitTask() = delete;

    explicit CommitTask(ViewSurface *surface)
        : surface_(surface) {}

    ~CommitTask() final = default;

    void Run() const final;

   private:

    ViewSurface *surface_;

  };

  /**
   * @brief A private structure for surface properties
   */
  struct Private;

  explicit ViewSurface(AbstractEventHandler *event_handler, const Margin &margin = Margin());

  void OnGLInterfaceDestroyed(__SLOT__);

  // global surface stack:

  /**
   * @brief Delete all shell surfaces and clear the global stack
   */
  static void Clear();

  /**
   * @brief The top shell surface in the stack
   */
  static ViewSurface *kTop;

  /**
   * @brief The bottom shell surface in the stack
   */
  static ViewSurface *kBottom;

  /**
   * @brief The count of shell surface
   */
  static int kShellSurfaceCount;

  static base::DequeT<RenderTask> kRenderTaskDeque;

  static base::DequeT<CommitTask> kCommitTaskDeque;

  std::unique_ptr<Private> p_;

};

/**
 * @brief Shell surface role
 */
class ViewSurface::Shell {

  friend class ViewSurface;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Shell);
  Shell() = delete;

  class Toplevel;
  class Popup;

  static Shell *Get(const ViewSurface *surface);

  void ResizeWindow(int width, int height) const;

  void AckConfigure(uint32_t serial) const;

  ViewSurface *surface() const { return surface_; }

 private:

  struct Private;

  static ViewSurface *Create(AbstractEventHandler *event_handler,
                         const Margin &margin = Margin());

  explicit Shell(ViewSurface *surface);

  ~Shell();

  void Push();

  void Remove();

  std::unique_ptr<Private> p_;

  ViewSurface *surface_;

  Shell *parent_;

  union {
    void *placeholder;
    Toplevel *toplevel;
    Popup *popup;
  } role_;

};

/**
 * @brief Toplevel shell surface role
 */
class ViewSurface::Shell::Toplevel {

  friend class Shell;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Toplevel);
  Toplevel() = delete;

  enum StatesMask {
    kStateMaskMaximized = 0x1,         /**< 1: the surface is maximized */
    kStateMaskFullscreen = 0x1 << 1,    /**< 2: the surface is fullscreen */
    kStateMaskResizing = 0x1 << 2,    /**< 4: the surface is being resized */
    kStateMaskActivated = 0x1 << 3,    /**< 8: the surface is now activated */
  };

  /**
   * @brief Create a toplevel shell surface
   */
  static ViewSurface *Create(AbstractEventHandler *event_handler,
                         const Margin &margin = Margin());

  static Toplevel *Get(const ViewSurface *surface);

  void SetTitle(const char *title) const;

  void SetAppId(const char *id) const;

  void Move(const InputEvent &input_event, uint32_t serial) const;

  void Resize(const InputEvent &input_event, uint32_t serial, uint32_t edges) const;

  void SetMaximized() const;

  void UnsetMaximized() const;

  void SetFullscreen(const Output *output) const;

  void UnsetFullscreen() const;

  void SetMinimized() const;

 private:

  struct Private;

  explicit Toplevel(Shell *shell);

  ~Toplevel();

  std::unique_ptr<Private> p_;

};

/**
 * @brief Popup shell surface role
 */
class ViewSurface::Shell::Popup {

  friend class Shell;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Popup);
  Popup() = delete;

  /**
   * @brief Create a popup shell surface
   */
  static ViewSurface *Create(ViewSurface *parent,
                         AbstractEventHandler *event_handler,
                         const Margin &margin = Margin());

  void SetSize(int32_t width, int32_t height);

  void SetAnchorRect(int32_t x, int32_t y, int32_t width, int32_t height);

  void SetAnchor(uint32_t anchor);

  void SetGravity(uint32_t gravity);

 private:

  struct Private;

  explicit Popup(Shell *shell);

  ~Popup();

  std::unique_ptr<Private> p_;

};

/**
 * @brief Sub surface role
 */
class ViewSurface::Sub {

  friend class ViewSurface;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Sub);
  Sub() = delete;

  /**
   * @brief Create a sub surface
   */
  static ViewSurface *Create(ViewSurface *parent,
                         AbstractEventHandler *event_handler,
                         const Margin &margin = Margin());

  static Sub *Get(const ViewSurface *surface);

  void PlaceAbove(ViewSurface *sibling);

  void PlaceBelow(ViewSurface *sibling);

  void SetRelativePosition(int x, int y);

  void SetWindowPosition(int x, int y);

  ViewSurface *surface() const { return surface_; }

 private:

  Sub(ViewSurface *surface, ViewSurface *parent);

  ~Sub();

  void SetParent(ViewSurface *parent);

  /**
 * @brief Move the local surface list and insert above target dst surface
 * @param dst
 */
  void MoveAbove(ViewSurface *dst);

  /**
   * @brief Move the local surface list and insert below target dst surface
   * @param dst
   */
  void MoveBelow(ViewSurface *dst);

  void InsertAbove(ViewSurface *sibling);

  void InsertBelow(ViewSurface *sibling);

  ViewSurface *surface_;

  struct wl_subsurface *wl_sub_surface_;

};

/**
 * @brief EGL surface role
 */

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_VIEW_SURFACE_HPP_
