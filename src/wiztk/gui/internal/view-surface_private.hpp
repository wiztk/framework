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

#ifndef WIZTK_GUI_INTERNAL_SURFACE_PRIVATE_HPP_
#define WIZTK_GUI_INTERNAL_SURFACE_PRIVATE_HPP_

#include "wiztk/gui/view-surface.hpp"
#include "wiztk/gui/abstract-rendering-api.hpp"

namespace wiztk {
namespace gui {

struct ViewSurface::Private {

  using PointI = base::PointI;

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);
  Private() = delete;

  Private(ViewSurface *surface, AbstractEventHandler *event_handler, const Margin &margin)
      : wl_surface(nullptr),
        commit_mode(kSynchronized),
        transform(kTransformNormal),
        scale(1),
        event_handler(event_handler),
        parent(nullptr),
        above(nullptr),
        below(nullptr),
        upper(nullptr),
        lower(nullptr),
        rendering_api(nullptr),
        render_task(surface),
        commit_task(surface) {
  }

  ~Private() = default;

  struct wl_surface *wl_surface;

  CommitMode commit_mode;
  Transform transform;
  int32_t scale;

  AbstractEventHandler *event_handler;

  /**
   * Position in parent surface
   *
   * For root surface, this should always be (0, 0)
   */
  PointI relative_position;

  /**
    * @brief The parent surface
    */
  ViewSurface *parent;

  /**
    * @brief The sibling surface placed up
    */
  ViewSurface *above;

  /**
    * @brief The sibling surface placed down
    */
  ViewSurface *below;

  /**
   * @brief The shell surface shows front
   */
  ViewSurface *upper;

  /**
   * @brief The shell surface shows back
   */
  ViewSurface *lower;

  AbstractRenderingAPI *rendering_api;

  union {
    void *placeholder;
    Shell *shell;
    Sub *sub;
  } role;

  RenderTask render_task;
  CommitTask commit_task;

  base::DequeT<AbstractView::RenderNode> view_render_deque;

  static void OnEnter(void *data, struct wl_surface *wl_surface,
                      struct wl_output *wl_output);

  static void OnLeave(void *data, struct wl_surface *wl_surface,
                      struct wl_output *wl_output);

  static const struct wl_surface_listener kListener;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_INTERNAL_SURFACE_PRIVATE_HPP_
