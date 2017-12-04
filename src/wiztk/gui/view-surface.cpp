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

#include "internal/view-surface_private.hpp"
#include "internal/view-surface_shell_private.hpp"
#include "internal/view-surface_shell_toplevel_private.hpp"
#include "internal/view-surface_shell_popup_private.hpp"

#include "wiztk/gui/buffer.hpp"
#include "wiztk/gui/input-event.hpp"
#include "wiztk/gui/region.hpp"

#include "internal/input_private.hpp"
#include "internal/output_private.hpp"
#include "internal/display_proxy.hpp"
#include "internal/buffer_private.hpp"

namespace wiztk {
namespace gui {

using Point = base::Point2I;
using base::Margin;

ViewSurface *ViewSurface::Shell::Create(AbstractEventHandler *event_handler, const Margin &margin) {
  auto *surface = new ViewSurface(event_handler, margin);
  surface->p_->role.shell = new Shell(surface);
  return surface;
}

ViewSurface::Shell *ViewSurface::Shell::Get(const ViewSurface *surface) {
  if (nullptr == surface->p_->parent)
    return surface->p_->role.shell;

  return nullptr;
}

void ViewSurface::Shell::ResizeWindow(int width, int height) const {
  zxdg_surface_v6_set_window_geometry(p_->zxdg_surface,
                                      surface_->margin().left,
                                      surface_->margin().top,
                                      width, height);
}

void ViewSurface::Shell::AckConfigure(uint32_t serial) const {
  zxdg_surface_v6_ack_configure(p_->zxdg_surface, serial);
}

ViewSurface::Shell::Shell(ViewSurface *surface)
    : surface_(surface), parent_(nullptr) {
  p_ = std::make_unique<Private>();

  _ASSERT(surface_);
  role_.placeholder = nullptr;

  p_->zxdg_surface = zxdg_shell_v6_get_xdg_surface(Display::Proxy::xdg_shell(), surface_->p_->wl_surface);
  zxdg_surface_v6_add_listener(p_->zxdg_surface, &Private::kListener, this);

  Push();
}

ViewSurface::Shell::~Shell() {
  Remove();

  if (nullptr == parent_) delete role_.toplevel;
  else delete role_.popup;

  _ASSERT(surface_->p_->role.shell == this);
  surface_->p_->role.shell = nullptr;
}

void ViewSurface::Shell::Push() {
  _ASSERT(nullptr == surface_->p_->parent);
  _ASSERT(nullptr == surface_->p_->upper);
  _ASSERT(nullptr == surface_->p_->lower);

  _ASSERT(ViewSurface::kShellSurfaceCount >= 0);

  if (nullptr != ViewSurface::kTop) {
    ViewSurface::kTop->p_->upper = surface_;
    surface_->p_->lower = ViewSurface::kTop;
    ViewSurface::kTop = surface_;
  } else {
    _ASSERT(ViewSurface::kShellSurfaceCount == 0);
    _ASSERT(nullptr == ViewSurface::kBottom);
    ViewSurface::kBottom = surface_;
    ViewSurface::kTop = surface_;
  }

  ViewSurface::kShellSurfaceCount++;
}

void ViewSurface::Shell::Remove() {
  _ASSERT(nullptr == surface_->p_->parent);

  if (nullptr != surface_->p_->upper) {
    surface_->p_->upper->p_->lower = surface_->p_->lower;
  } else {
    _ASSERT(ViewSurface::kTop == surface_);
    ViewSurface::kTop = surface_->p_->lower;
  }

  if (nullptr != surface_->p_->lower) {
    surface_->p_->lower->p_->upper = surface_->p_->upper;
  } else {
    _ASSERT(ViewSurface::kBottom == surface_);
    ViewSurface::kBottom = surface_->p_->upper;
  }

  surface_->p_->upper = nullptr;
  surface_->p_->lower = nullptr;
  ViewSurface::kShellSurfaceCount--;
  _ASSERT(ViewSurface::kShellSurfaceCount >= 0);
}

// ------

ViewSurface *ViewSurface::Shell::Toplevel::Create(AbstractEventHandler *event_handler, const Margin &margin) {
  ViewSurface *surface = Shell::Create(event_handler, margin);
  Shell *shell = Shell::Get(surface);
  shell->role_.toplevel = new Toplevel(shell);
  return surface;
}

ViewSurface::Shell::Toplevel *ViewSurface::Shell::Toplevel::Get(const ViewSurface *surface) {
  Shell *shell = Shell::Get(surface);
  if ((nullptr == shell) || (nullptr != shell->parent_)) return nullptr;
  return shell->role_.toplevel;
}

void ViewSurface::Shell::Toplevel::SetTitle(const char *title) const {
  zxdg_toplevel_v6_set_title(p_->zxdg_toplevel, title);
}

void ViewSurface::Shell::Toplevel::SetAppId(const char *id) const {
  zxdg_toplevel_v6_set_app_id(p_->zxdg_toplevel, id);
}

void ViewSurface::Shell::Toplevel::Move(const InputEvent &input_event, uint32_t serial) const {
  zxdg_toplevel_v6_move(p_->zxdg_toplevel, input_event.input_->p_->wl_seat, serial);
}

void ViewSurface::Shell::Toplevel::Resize(const InputEvent &input_event, uint32_t serial, uint32_t edges) const {
  zxdg_toplevel_v6_resize(p_->zxdg_toplevel, input_event.input_->p_->wl_seat, serial, edges);
}

void ViewSurface::Shell::Toplevel::SetMaximized() const {
  zxdg_toplevel_v6_set_maximized(p_->zxdg_toplevel);
}

void ViewSurface::Shell::Toplevel::UnsetMaximized() const {
  zxdg_toplevel_v6_unset_maximized(p_->zxdg_toplevel);
}

void ViewSurface::Shell::Toplevel::SetFullscreen(const Output *output) const {
  zxdg_toplevel_v6_set_fullscreen(p_->zxdg_toplevel, output->p_->wl_output);
}

void ViewSurface::Shell::Toplevel::UnsetFullscreen() const {
  zxdg_toplevel_v6_unset_fullscreen(p_->zxdg_toplevel);
}

void ViewSurface::Shell::Toplevel::SetMinimized() const {
  zxdg_toplevel_v6_set_minimized(p_->zxdg_toplevel);
}

ViewSurface::Shell::Toplevel::Toplevel(Shell *shell) {
  _ASSERT(nullptr != shell);
  p_ = std::make_unique<Private>();
  p_->shell = shell;

  p_->zxdg_toplevel = zxdg_surface_v6_get_toplevel(shell->p_->zxdg_surface);
  zxdg_toplevel_v6_add_listener(p_->zxdg_toplevel, &Private::kListener, this);
}

ViewSurface::Shell::Toplevel::~Toplevel() {
  _ASSERT(p_->shell->role_.toplevel == this);
  _ASSERT(nullptr == p_->shell->parent_);
  p_->shell->role_.toplevel = nullptr;
}

// ------

ViewSurface *ViewSurface::Shell::Popup::Create(ViewSurface *parent, AbstractEventHandler *event_handler, const Margin &margin) {
  if (nullptr == parent) throw std::runtime_error("Error! parent is nullptr!");

  if (nullptr == Shell::Get(parent)) throw std::runtime_error("Error! parent is not a shell surface!");

  ViewSurface *surface = Shell::Create(event_handler, margin);
  Shell *shell = Shell::Get(surface);
  shell->parent_ = parent->p_->role.shell;
  shell->role_.popup = new Popup(shell);
  return surface;
}

ViewSurface::Shell::Popup::Popup(Shell *shell) {
  _ASSERT(nullptr != shell);
  p_ = std::make_unique<Private>();
  p_->shell = shell;

  p_->zxdg_positioner = zxdg_shell_v6_create_positioner(Display::Proxy::xdg_shell());
  p_->zxdg_popup = zxdg_surface_v6_get_popup(shell->p_->zxdg_surface,
                                             shell->parent_->p_->zxdg_surface,
                                             p_->zxdg_positioner);
}

ViewSurface::Shell::Popup::~Popup() {
  _ASSERT(p_->shell->parent_);
  _ASSERT(p_->shell->role_.popup == this);

  p_->shell->parent_ = nullptr;
  p_->shell->role_.popup = nullptr;
}

// ------

ViewSurface *ViewSurface::Sub::Create(ViewSurface *parent, AbstractEventHandler *event_handler, const Margin &margin) {
  auto *surface = new ViewSurface(event_handler, margin);
  surface->p_->role.sub = new Sub(surface, parent);
  return surface;
}

ViewSurface::Sub *ViewSurface::Sub::Get(const ViewSurface *surface) {
  if (nullptr == surface->p_->parent)
    return nullptr;

  return surface->p_->role.sub;
}

ViewSurface::Sub::Sub(ViewSurface *surface, ViewSurface *parent)
    : surface_(surface), wl_sub_surface_(nullptr) {
  _ASSERT(nullptr != surface_);
  _ASSERT(nullptr != parent);

  wl_sub_surface_ = wl_subcompositor_get_subsurface(Display::Proxy::wl_subcompositor(),
                                                    surface_->p_->wl_surface,
                                                    parent->p_->wl_surface);
  SetParent(parent);
}

ViewSurface::Sub::~Sub() {
  _ASSERT(surface_->p_->role.sub == this);

  // Delete all sub surfaces of this one:
  ViewSurface *p = nullptr;
  ViewSurface *tmp = nullptr;

  p = surface_->p_->above;
  while (p && p->p_->parent == surface_) {
    tmp = p->p_->above;
    delete p;
    p = tmp;
  }

  p = surface_->p_->below;
  while (p && p->p_->parent == surface_) {
    tmp = p->p_->below;
    delete p;
    p = tmp;
  }

  // Break the link node
  if (nullptr != surface_->p_->above) surface_->p_->above->p_->below = surface_->p_->below;
  if (nullptr != surface_->p_->below) surface_->p_->below->p_->above = surface_->p_->above;

  if (nullptr != wl_sub_surface_)
    wl_subsurface_destroy(wl_sub_surface_);

  surface_->p_->role.sub = nullptr;
}

void ViewSurface::Sub::PlaceAbove(ViewSurface *sibling) {
  if (sibling == surface_) return;

  if (surface_->GetParent() == sibling->GetParent() ||
      surface_ == sibling->GetParent() ||
      surface_->GetParent() == sibling) {
    wl_subsurface_place_above(wl_sub_surface_, sibling->p_->wl_surface);
    MoveAbove(sibling);
  }
}

void ViewSurface::Sub::PlaceBelow(ViewSurface *sibling) {
  if (sibling == surface_) return;

  if (surface_->GetParent() == sibling->GetParent() ||
      surface_ == sibling->GetParent() ||
      surface_->GetParent() == sibling) {
    wl_subsurface_place_below(wl_sub_surface_, sibling->p_->wl_surface);
    MoveBelow(sibling);
  }
}

void ViewSurface::Sub::SetRelativePosition(int x, int y) {
  wl_subsurface_set_position(wl_sub_surface_, x, y);
  surface_->p_->relative_position.x = x;
  surface_->p_->relative_position.y = y;
}

void ViewSurface::Sub::SetWindowPosition(int x, int y) {
  Point parent_global_position = surface_->GetParent()->GetWindowPosition();
  int local_x = x - parent_global_position.x;
  int local_y = y - parent_global_position.y;
  wl_subsurface_set_position(wl_sub_surface_, local_x, local_y);
  surface_->p_->relative_position.x = x;
  surface_->p_->relative_position.y = y;
}

void ViewSurface::Sub::SetParent(ViewSurface *parent) {
  _ASSERT(nullptr == surface_->p_->parent &&
      surface_->p_->upper == nullptr &&
      surface_->p_->lower == nullptr);

  surface_->p_->parent = parent;

  ViewSurface *tmp = parent;
  ViewSurface *sibling = nullptr;
  do {
    sibling = tmp;
    tmp = tmp->p_->above;
    if (nullptr == tmp || tmp->p_->parent != parent) break;
  } while (true);
  InsertAbove(sibling);
}

void ViewSurface::Sub::MoveAbove(ViewSurface *dst) {
  ViewSurface *top = surface_;
  ViewSurface *bottom = surface_;
  ViewSurface *tmp = nullptr;

  tmp = surface_;
  while (tmp->p_->above && (tmp->p_->above->p_->parent != surface_->p_->parent)) {
    top = tmp;
    tmp = tmp->p_->above;
  }

  tmp = surface_;
  while (tmp->p_->below && (tmp->p_->below->p_->parent != surface_->p_->parent)) {
    bottom = tmp;
    tmp = tmp->p_->below;
  }

  if (top == bottom) {
    if (surface_->p_->above) surface_->p_->above->p_->below = surface_->p_->below;
    if (surface_->p_->below) surface_->p_->below->p_->above = surface_->p_->above;

    surface_->p_->above = dst->p_->above;
    surface_->p_->below = dst;
    if (dst->p_->above) dst->p_->above->p_->below = surface_;
    dst->p_->above = surface_;
  } else {
    if (top->p_->above) top->p_->above->p_->below = bottom->p_->below;
    if (bottom->p_->below) bottom->p_->below->p_->above = top->p_->above;

    top->p_->above = dst->p_->above;
    bottom->p_->below = dst;
    if (dst->p_->above) dst->p_->above->p_->below = top;
    dst->p_->above = bottom;
  }
}

void ViewSurface::Sub::MoveBelow(ViewSurface *dst) {
  ViewSurface *top = surface_;
  ViewSurface *bottom = surface_;
  ViewSurface *tmp = nullptr;

  tmp = surface_;
  while (tmp->p_->above && (tmp->p_->above->p_->parent != surface_->p_->parent)) {
    top = tmp;
    tmp = tmp->p_->above;
  }

  tmp = surface_;
  while (tmp->p_->below && (tmp->p_->below->p_->parent != surface_->p_->parent)) {
    bottom = tmp;
    tmp = tmp->p_->below;
  }

  if (top == bottom) {
    if (surface_->p_->above) surface_->p_->above->p_->below = surface_->p_->below;
    if (surface_->p_->below) surface_->p_->below->p_->above = surface_->p_->above;

    surface_->p_->above = dst;
    surface_->p_->below = dst->p_->below;
    if (dst->p_->below) dst->p_->below->p_->above = surface_;
    dst->p_->below = surface_;
  } else {
    if (top->p_->above) top->p_->above->p_->below = bottom->p_->below;
    if (bottom->p_->below) bottom->p_->below->p_->above = top->p_->above;

    top->p_->above = dst;
    bottom->p_->below = dst->p_->below;
    if (dst->p_->below) dst->p_->below->p_->above = bottom;
    dst->p_->below = top;
  }
}

void ViewSurface::Sub::InsertAbove(ViewSurface *sibling) {
  _ASSERT(surface_->p_->parent == sibling->p_->parent ||
      surface_ == sibling->p_->parent ||
      surface_->p_->parent == sibling);
  if (sibling->p_->above) sibling->p_->above->p_->below = surface_;
  surface_->p_->above = sibling->p_->above;
  sibling->p_->above = surface_;
  surface_->p_->below = sibling;
}

void ViewSurface::Sub::InsertBelow(ViewSurface *sibling) {
  _ASSERT(surface_->p_->parent == sibling->p_->parent ||
      surface_ == sibling->p_->parent ||
      surface_->p_->parent == sibling);
  if (sibling->p_->below) sibling->p_->below->p_->above = surface_;
  surface_->p_->below = sibling->p_->below;
  sibling->p_->below = surface_;
  surface_->p_->above = sibling;
}

// ------

void ViewSurface::RenderTask::Run() const {
  surface_->p_->event_handler->OnRenderSurface(surface_);
}

void ViewSurface::CommitTask::Run() const {
  wl_surface_commit(surface_->p_->wl_surface);
}

// ------

ViewSurface *ViewSurface::kTop = nullptr;
ViewSurface *ViewSurface::kBottom = nullptr;
int ViewSurface::kShellSurfaceCount = 0;
base::Deque<ViewSurface::RenderTask> ViewSurface::kRenderTaskDeque;
base::Deque<ViewSurface::CommitTask> ViewSurface::kCommitTaskDeque;

ViewSurface::ViewSurface(AbstractEventHandler *event_handler, const Margin &margin)
    : AbstractSurface(margin) {
  _ASSERT(nullptr != event_handler);
  p_ = std::make_unique<Private>(this, event_handler, margin);
  p_->role.placeholder = nullptr;

  p_->wl_surface = wl_compositor_create_surface(Display::Proxy::wl_compositor());
  wl_surface_add_listener(p_->wl_surface, &Private::kListener, this);
}

ViewSurface::~ViewSurface() {
  if (nullptr != p_->rendering_api) {
    p_->rendering_api->Release(this);
  }

//  if (p_->egl)
//    delete p_->egl;

  if (nullptr == p_->parent)
    delete p_->role.shell; // deleting a shell surface will break the links to up_ and down_
  else
    delete p_->role.sub; // deleting all sub surfaces and break the links to above_ and below_

  if (nullptr != p_->wl_surface)
    wl_surface_destroy(p_->wl_surface);
}

void ViewSurface::Attach(Buffer *buffer, int32_t x, int32_t y) {
  if (nullptr == buffer) {
    wl_surface_attach(p_->wl_surface, NULL, x, y);
    return;
  }

  buffer->SetPosition(x, y);
  wl_surface_attach(p_->wl_surface, buffer->p_->wl_buffer, x, y);
}

void ViewSurface::Commit() {
  if (nullptr != p_->rendering_api) {
    // GL surface does not use commit
    if (p_->commit_mode == kSynchronized) {
      ViewSurface *main_surface = GetShellSurface();
      if (main_surface != this)
        main_surface->Commit();
    }

    return;
  }

  if (p_->commit_task.IsLinked())
    return;

  if (nullptr == p_->parent) {
    kCommitTaskDeque.PushBack(&p_->commit_task);
    return;
  }

  if (p_->commit_mode == kSynchronized) {
    // Synchronized mode need to commit the main surface too
    ViewSurface *main_surface = GetShellSurface();
    main_surface->Commit();
    main_surface->p_->commit_task.PushFront(&p_->commit_task);
  } else {
    kCommitTaskDeque.PushBack(&p_->commit_task);
  }
}

void ViewSurface::SetCommitMode(CommitMode mode) {
  p_->commit_mode = mode;
}

void ViewSurface::Damage(int surface_x, int surface_y, int width, int height) {
  wl_surface_damage(p_->wl_surface, surface_x, surface_y, width, height);
}

void ViewSurface::SetInputRegion(const Region &region) {
  wl_surface_set_input_region(p_->wl_surface, region.wl_region_);
}

void ViewSurface::SetOpaqueRegion(const Region &region) {
  wl_surface_set_opaque_region(p_->wl_surface, region.wl_region_);
}

void ViewSurface::SetTransform(Transform transform) {
  if (p_->transform != transform) {
    wl_surface_set_buffer_transform(p_->wl_surface, transform);
    p_->transform = transform;
  }
}

ViewSurface::Transform ViewSurface::GetTransform() const {
  return p_->transform;
}

void ViewSurface::SetScale(int32_t scale) {
  if (p_->scale != scale) {
    wl_surface_set_buffer_scale(p_->wl_surface, scale);
    p_->scale = scale;
  }
}

int32_t ViewSurface::GetScale() const {
  return p_->scale;
}

void ViewSurface::DamageBuffer(int32_t x, int32_t y, int32_t width, int32_t height) {
  wl_surface_damage_buffer(p_->wl_surface, x, y, width, height);
}

void ViewSurface::Update(bool validate) {
  if (!validate) {
    p_->render_task.Unlink();
    return;
  }

  if (p_->render_task.IsLinked()) return;
  kRenderTaskDeque.PushBack(&p_->render_task);
}

base::Deque<AbstractView::RenderNode> &ViewSurface::GetViewRenderDeque() const {
  return p_->view_render_deque;
}

ViewSurface *ViewSurface::GetShellSurface() {
  ViewSurface *shell_surface = this;
  ViewSurface *parent = p_->parent;

  while (nullptr != parent) {
    shell_surface = parent;
    parent = parent->p_->parent;
  }

  return shell_surface;
}

Point ViewSurface::GetWindowPosition() const {
  Point position = p_->relative_position;

  const ViewSurface *parent = p_->parent;
  const ViewSurface *shell_surface = this;

  while (nullptr != parent) {
    position += parent->GetRelativePosition();
    if (nullptr == parent->p_->parent) shell_surface = parent;
    parent = parent->GetParent();
  }

  return position - Point(shell_surface->margin().l, shell_surface->margin().t);
}

ViewSurface *ViewSurface::GetParent() const {
  return p_->parent;
}

ViewSurface *ViewSurface::GetSiblingAbove() const {
  return p_->above;
}

ViewSurface *ViewSurface::GetSiblingBelow() const {
  return p_->below;
}

ViewSurface *ViewSurface::GetUpperShell() const {
  return p_->upper;
}

ViewSurface *ViewSurface::GetLowerShell() const {
  return p_->lower;
}

AbstractEventHandler *ViewSurface::GetEventHandler() const {
  return p_->event_handler;
}

void ViewSurface::SetRenderingAPI(AbstractRenderingAPI *api) {
  api->Setup(this);
  api->destroyed().Connect(this, &ViewSurface::OnGLInterfaceDestroyed);
}

AbstractRenderingAPI *ViewSurface::GetRenderingAPI() const {
  return p_->rendering_api;
}

const Point &ViewSurface::GetRelativePosition() const {
  return p_->relative_position;
}

void ViewSurface::Render(Delegate *delegate) {
  if (nullptr != delegate) delegate->Render();
  // TODO: use this method
}

void ViewSurface::OnGLInterfaceDestroyed(base::SLOT /* slot */) {
  p_->rendering_api = nullptr;
}

void ViewSurface::Clear() {
  while (kShellSurfaceCount > 0) {
    AbstractEventHandler *event_handler = kTop->GetEventHandler();
    delete event_handler;
  }
}

} // namespace gui
} // namespace wiztk
