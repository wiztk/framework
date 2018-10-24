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

#include "surface/private.hpp"
#include "surface/shell/private.hpp"
#include "surface/shell/toplevel/private.hpp"
#include "surface/shell/popup/private.hpp"
#include "input/private.hpp"
#include "output/private.hpp"
#include "display/private.hpp"
#include "buffer/private.hpp"

#include "wiztk/gui/buffer.hpp"
#include "wiztk/gui/input-event.hpp"
#include "wiztk/gui/region.hpp"
#include "wiztk/gui/application.hpp"

namespace wiztk {
namespace gui {

using Point = base::Point2I;

Surface *Surface::Shell::Create(AbstractEventHandler *event_handler,
                                AbstractRenderingBackend *backend,
                                const Margin &margin) {
  auto *surface = new Surface(event_handler, backend, margin);
  surface->p_->role.shell = new Shell(surface);
  return surface;
}

Surface::Shell *Surface::Shell::Get(const Surface *surface) {
  if (nullptr == surface->p_->parent)
    return surface->p_->role.shell;

  return nullptr;
}

void Surface::Shell::ResizeWindow(int width, int height) const {
  zxdg_surface_v6_set_window_geometry(p_->zxdg_surface,
                                      surface_->GetMargin().left,
                                      surface_->GetMargin().top,
                                      width, height);
}

void Surface::Shell::AckConfigure(uint32_t serial) const {
  zxdg_surface_v6_ack_configure(p_->zxdg_surface, serial);
}

Surface::Shell::Shell(Surface *surface)
    : surface_(surface), parent_(nullptr) {
  p_ = std::make_unique<Private>();

  _ASSERT(surface_);
  role_.placeholder = nullptr;

  Display *display = Application::GetInstance()->GetDisplay();
  p_->zxdg_surface = zxdg_shell_v6_get_xdg_surface(Display::Private::Get(*display).xdg_shell,
                                                   surface_->p_->wl_surface);
  zxdg_surface_v6_add_listener(p_->zxdg_surface, &Private::kListener, this);

  Push();
}

Surface::Shell::~Shell() {
  Remove();

  if (nullptr == parent_) delete role_.toplevel;
  else delete role_.popup;

  _ASSERT(surface_->p_->role.shell == this);
  surface_->p_->role.shell = nullptr;
}

void Surface::Shell::Push() {
  _ASSERT(nullptr == surface_->p_->parent);
  _ASSERT(nullptr == surface_->p_->upper);
  _ASSERT(nullptr == surface_->p_->lower);

  _ASSERT(Surface::kShellSurfaceCount >= 0);

  if (nullptr != Surface::kTop) {
    Surface::kTop->p_->upper = surface_;
    surface_->p_->lower = Surface::kTop;
    Surface::kTop = surface_;
  } else {
    _ASSERT(Surface::kShellSurfaceCount == 0);
    _ASSERT(nullptr == Surface::kBottom);
    Surface::kBottom = surface_;
    Surface::kTop = surface_;
  }

  Surface::kShellSurfaceCount++;
}

void Surface::Shell::Remove() {
  _ASSERT(nullptr == surface_->p_->parent);

  if (nullptr != surface_->p_->upper) {
    surface_->p_->upper->p_->lower = surface_->p_->lower;
  } else {
    _ASSERT(Surface::kTop == surface_);
    Surface::kTop = surface_->p_->lower;
  }

  if (nullptr != surface_->p_->lower) {
    surface_->p_->lower->p_->upper = surface_->p_->upper;
  } else {
    _ASSERT(Surface::kBottom == surface_);
    Surface::kBottom = surface_->p_->upper;
  }

  surface_->p_->upper = nullptr;
  surface_->p_->lower = nullptr;
  Surface::kShellSurfaceCount--;
  _ASSERT(Surface::kShellSurfaceCount >= 0);
}

// ------

Surface *Surface::Shell::Toplevel::Create(AbstractEventHandler *event_handler,
                                          AbstractRenderingBackend *backend,
                                          const Margin &margin) {
  Surface *surface = Shell::Create(event_handler, backend, margin);
  Shell *shell = Shell::Get(surface);
  shell->role_.toplevel = new Toplevel(shell);
  return surface;
}

Surface::Shell::Toplevel *Surface::Shell::Toplevel::Get(const Surface *surface) {
  Shell *shell = Shell::Get(surface);
  if ((nullptr == shell) || (nullptr != shell->parent_)) return nullptr;
  return shell->role_.toplevel;
}

void Surface::Shell::Toplevel::SetTitle(const char *title) const {
  zxdg_toplevel_v6_set_title(p_->zxdg_toplevel, title);
}

void Surface::Shell::Toplevel::SetAppId(const char *id) const {
  zxdg_toplevel_v6_set_app_id(p_->zxdg_toplevel, id);
}

void Surface::Shell::Toplevel::Move(const InputEvent &input_event, uint32_t serial) const {
  zxdg_toplevel_v6_move(p_->zxdg_toplevel, input_event.input_->p_->wl_seat, serial);
}

void Surface::Shell::Toplevel::Resize(const InputEvent &input_event, uint32_t serial, uint32_t edges) const {
  zxdg_toplevel_v6_resize(p_->zxdg_toplevel, input_event.input_->p_->wl_seat, serial, edges);
}

void Surface::Shell::Toplevel::SetMaximized() const {
  zxdg_toplevel_v6_set_maximized(p_->zxdg_toplevel);
}

void Surface::Shell::Toplevel::UnsetMaximized() const {
  zxdg_toplevel_v6_unset_maximized(p_->zxdg_toplevel);
}

void Surface::Shell::Toplevel::SetFullscreen(const Output *output) const {
  zxdg_toplevel_v6_set_fullscreen(p_->zxdg_toplevel, output->p_->wl_output);
}

void Surface::Shell::Toplevel::UnsetFullscreen() const {
  zxdg_toplevel_v6_unset_fullscreen(p_->zxdg_toplevel);
}

void Surface::Shell::Toplevel::SetMinimized() const {
  zxdg_toplevel_v6_set_minimized(p_->zxdg_toplevel);
}

Surface::Shell::Toplevel::Toplevel(Shell *shell) {
  _ASSERT(nullptr != shell);
  p_ = std::make_unique<Private>();
  p_->shell = shell;

  p_->zxdg_toplevel = zxdg_surface_v6_get_toplevel(shell->p_->zxdg_surface);
  zxdg_toplevel_v6_add_listener(p_->zxdg_toplevel, &Private::kListener, this);
}

Surface::Shell::Toplevel::~Toplevel() {
  _ASSERT(p_->shell->role_.toplevel == this);
  _ASSERT(nullptr == p_->shell->parent_);
  p_->shell->role_.toplevel = nullptr;
}

// ------

Surface *Surface::Shell::Popup::Create(Surface *parent,
                                       AbstractEventHandler *event_handler,
                                       AbstractRenderingBackend *backend,
                                       const Margin &margin) {
  if (nullptr == parent) throw std::runtime_error("Error! parent is nullptr!");

  if (nullptr == Shell::Get(parent)) throw std::runtime_error("Error! parent is not a shell surface!");

  Surface *surface = Shell::Create(event_handler, nullptr, margin);
  Shell *shell = Shell::Get(surface);
  shell->parent_ = parent->p_->role.shell;
  shell->role_.popup = new Popup(shell);
  return surface;
}

Surface::Shell::Popup::Popup(Shell *shell) {
  _ASSERT(nullptr != shell);
  p_ = std::make_unique<Private>();
  p_->shell = shell;

  Display *display = Application::GetInstance()->GetDisplay();
  p_->zxdg_positioner = zxdg_shell_v6_create_positioner(Display::Private::Get(*display).xdg_shell);
  p_->zxdg_popup = zxdg_surface_v6_get_popup(shell->p_->zxdg_surface,
                                             shell->parent_->p_->zxdg_surface,
                                             p_->zxdg_positioner);
}

Surface::Shell::Popup::~Popup() {
  _ASSERT(p_->shell->parent_);
  _ASSERT(p_->shell->role_.popup == this);

  p_->shell->parent_ = nullptr;
  p_->shell->role_.popup = nullptr;
}

// ------

Surface *Surface::Sub::Create(Surface *parent,
                              AbstractEventHandler *event_handler,
                              AbstractRenderingBackend *backend,
                              const Margin &margin) {
  auto *surface = new Surface(event_handler, backend, margin);
  surface->p_->role.sub = new Sub(surface, parent);
  return surface;
}

Surface::Sub *Surface::Sub::Get(const Surface *surface) {
  if (nullptr == surface->p_->parent)
    return nullptr;

  return surface->p_->role.sub;
}

Surface::Sub::Sub(Surface *surface, Surface *parent)
    : surface_(surface), wl_sub_surface_(nullptr) {
  _ASSERT(nullptr != surface_);
  _ASSERT(nullptr != parent);

  Display *display = Application::GetInstance()->GetDisplay();
  wl_sub_surface_ = wl_subcompositor_get_subsurface(Display::Private::Get(*display).wl_subcompositor,
                                                    surface_->p_->wl_surface,
                                                    parent->p_->wl_surface);
  SetParent(parent);
}

Surface::Sub::~Sub() {
  _ASSERT(surface_->p_->role.sub == this);

  // Delete all sub surfaces of this one:
  Surface *p = nullptr;
  Surface *tmp = nullptr;

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

void Surface::Sub::PlaceAbove(Surface *sibling) {
  if (sibling == surface_) return;

  if (surface_->GetParent() == sibling->GetParent() ||
      surface_ == sibling->GetParent() ||
      surface_->GetParent() == sibling) {
    wl_subsurface_place_above(wl_sub_surface_, sibling->p_->wl_surface);
    MoveAbove(sibling);
  }
}

void Surface::Sub::PlaceBelow(Surface *sibling) {
  if (sibling == surface_) return;

  if (surface_->GetParent() == sibling->GetParent() ||
      surface_ == sibling->GetParent() ||
      surface_->GetParent() == sibling) {
    wl_subsurface_place_below(wl_sub_surface_, sibling->p_->wl_surface);
    MoveBelow(sibling);
  }
}

void Surface::Sub::SetRelativePosition(int x, int y) {
  wl_subsurface_set_position(wl_sub_surface_, x, y);
  surface_->p_->relative_position.x = x;
  surface_->p_->relative_position.y = y;
}

void Surface::Sub::SetWindowPosition(int x, int y) {
  Point parent_global_position = surface_->GetParent()->GetWindowPosition();
  int local_x = x - parent_global_position.x;
  int local_y = y - parent_global_position.y;
  wl_subsurface_set_position(wl_sub_surface_, local_x, local_y);
  surface_->p_->relative_position.x = x;
  surface_->p_->relative_position.y = y;
}

void Surface::Sub::SetParent(Surface *parent) {
  _ASSERT(nullptr == surface_->p_->parent &&
      surface_->p_->upper == nullptr &&
      surface_->p_->lower == nullptr);

  surface_->p_->parent = parent;

  Surface *tmp = parent;
  Surface *sibling = nullptr;
  do {
    sibling = tmp;
    tmp = tmp->p_->above;
    if (nullptr == tmp || tmp->p_->parent != parent) break;
  } while (true);
  InsertAbove(sibling);
}

void Surface::Sub::MoveAbove(Surface *dst) {
  Surface *top = surface_;
  Surface *bottom = surface_;
  Surface *tmp = nullptr;

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

void Surface::Sub::MoveBelow(Surface *dst) {
  Surface *top = surface_;
  Surface *bottom = surface_;
  Surface *tmp = nullptr;

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

void Surface::Sub::InsertAbove(Surface *sibling) {
  _ASSERT(surface_->p_->parent == sibling->p_->parent ||
      surface_ == sibling->p_->parent ||
      surface_->p_->parent == sibling);
  if (sibling->p_->above) sibling->p_->above->p_->below = surface_;
  surface_->p_->above = sibling->p_->above;
  sibling->p_->above = surface_;
  surface_->p_->below = sibling;
}

void Surface::Sub::InsertBelow(Surface *sibling) {
  _ASSERT(surface_->p_->parent == sibling->p_->parent ||
      surface_ == sibling->p_->parent ||
      surface_->p_->parent == sibling);
  if (sibling->p_->below) sibling->p_->below->p_->above = surface_;
  surface_->p_->below = sibling->p_->below;
  sibling->p_->below = surface_;
  surface_->p_->above = sibling;
}

// ------

void Surface::RenderTask::Run() {
  surface_->p_->event_handler->OnRenderSurface(surface_);
}

void Surface::CommitTask::Run() {
  wl_surface_commit(surface_->p_->wl_surface);
}

// ------

Surface *Surface::kTop = nullptr;
Surface *Surface::kBottom = nullptr;
int Surface::kShellSurfaceCount = 0;
base::Deque<Surface::RenderTask> Surface::kRenderTaskDeque;
base::Deque<Surface::CommitTask> Surface::kCommitTaskDeque;

Surface::Surface(AbstractEventHandler *event_handler, AbstractRenderingBackend *backend, const Margin &margin) {
  _ASSERT(nullptr != event_handler);
  p_ = std::make_unique<Private>(this, event_handler, margin);
  p_->role.placeholder = nullptr;
  p_->rendering_backend = backend;

  Display *display = Application::GetInstance()->GetDisplay();
  p_->wl_surface = wl_compositor_create_surface(Display::Private::Get(*display).wl_compositor);
  wl_surface_add_listener(p_->wl_surface, &Private::kListener, this);
}

Surface::~Surface() {
  if (nullptr != p_->rendering_api) {
    p_->rendering_api->Release(this);
  }

  delete p_->rendering_backend;

//  if (p_->egl)
//    delete p_->egl;

  if (nullptr == p_->parent)
    delete p_->role.shell; // deleting a shell surface will break the links to up_ and down_
  else
    delete p_->role.sub; // deleting all sub surfaces and break the links to above_ and below_

  if (nullptr != p_->wl_surface)
    wl_surface_destroy(p_->wl_surface);
}

const Surface::Margin &Surface::GetMargin() const {
  return p_->margin;
}

void Surface::Attach(Buffer *buffer, int32_t x, int32_t y) {
  if (nullptr == buffer) {
    wl_surface_attach(p_->wl_surface, NULL, x, y);
    return;
  }

  buffer->SetPosition(x, y);
  wl_surface_attach(p_->wl_surface, buffer->p_->wl_buffer, x, y);
}

void Surface::Commit() {
  if (nullptr != p_->rendering_api) {
    // GL surface does not use commit
    if (p_->commit_mode == kSynchronized) {
      Surface *main_surface = GetShellSurface();
      if (main_surface != this)
        main_surface->Commit();
    }

    return;
  }

  if (p_->commit_task.is_linked())
    return;

  if (nullptr == p_->parent) {
    kCommitTaskDeque.push_back(&p_->commit_task);
    return;
  }

  if (p_->commit_mode == kSynchronized) {
    // Synchronized mode need to commit the main surface too
    Surface *main_surface = GetShellSurface();
    main_surface->Commit();
    main_surface->p_->commit_task.push_back(&p_->commit_task);
  } else {
    kCommitTaskDeque.push_back(&p_->commit_task);
  }
}

void Surface::SetCommitMode(CommitMode mode) {
  p_->commit_mode = mode;
}

void Surface::Damage(int surface_x, int surface_y, int width, int height) {
  wl_surface_damage(p_->wl_surface, surface_x, surface_y, width, height);
}

void Surface::SetInputRegion(const Region &region) {
  wl_surface_set_input_region(p_->wl_surface, region.wl_region_);
}

void Surface::SetOpaqueRegion(const Region &region) {
  wl_surface_set_opaque_region(p_->wl_surface, region.wl_region_);
}

void Surface::SetTransform(Transform transform) {
  if (p_->transform != transform) {
    wl_surface_set_buffer_transform(p_->wl_surface, transform);
    p_->transform = transform;
  }
}

Surface::Transform Surface::GetTransform() const {
  return p_->transform;
}

void Surface::SetScale(int32_t scale) {
  if (p_->scale != scale) {
    wl_surface_set_buffer_scale(p_->wl_surface, scale);
    p_->scale = scale;
  }
}

int32_t Surface::GetScale() const {
  return p_->scale;
}

void Surface::DamageBuffer(int32_t x, int32_t y, int32_t width, int32_t height) {
  wl_surface_damage_buffer(p_->wl_surface, x, y, width, height);
}

void Surface::Update(bool validate) {
  if (!validate) {
    p_->render_task.unlink();
    return;
  }

  if (p_->render_task.is_linked()) return;
  kRenderTaskDeque.push_back(&p_->render_task);
}

base::Deque<AbstractView::RenderNode> &Surface::GetRenderDeque() const {
  return p_->render_deque;
}

Surface *Surface::GetShellSurface() {
  Surface *shell_surface = this;
  Surface *parent = p_->parent;

  while (nullptr != parent) {
    shell_surface = parent;
    parent = parent->p_->parent;
  }

  return shell_surface;
}

Point Surface::GetWindowPosition() const {
  Point position = p_->relative_position;

  const Surface *parent = p_->parent;
  const Surface *shell_surface = this;

  while (nullptr != parent) {
    position += parent->GetRelativePosition();
    if (nullptr == parent->p_->parent) shell_surface = parent;
    parent = parent->GetParent();
  }

  return position - Point(shell_surface->GetMargin().l, shell_surface->GetMargin().t);
}

Surface *Surface::GetParent() const {
  return p_->parent;
}

Surface *Surface::GetSiblingAbove() const {
  return p_->above;
}

Surface *Surface::GetSiblingBelow() const {
  return p_->below;
}

Surface *Surface::GetUpperShell() const {
  return p_->upper;
}

Surface *Surface::GetLowerShell() const {
  return p_->lower;
}

AbstractEventHandler *Surface::GetEventHandler() const {
  return p_->event_handler;
}

void Surface::SetRenderingAPI(AbstractRenderingAPI *api) {
  api->Setup(this);
  api->destroyed().Connect(this, &Surface::OnGLInterfaceDestroyed);
}

AbstractRenderingAPI *Surface::GetRenderingAPI() const {
  return p_->rendering_api;
}

void Surface::SetRenderingBackend(AbstractRenderingBackend *backend) {
  if (p_->rendering_backend == backend) return;

  delete p_->rendering_backend;
  p_->rendering_backend = backend;
}

AbstractRenderingBackend *Surface::GetRenderingBackend() const {
  return p_->rendering_backend;
}

const Point &Surface::GetRelativePosition() const {
  return p_->relative_position;
}

void Surface::OnGLInterfaceDestroyed(base::SLOT /* slot */) {
  p_->rendering_api = nullptr;
}

void Surface::Clear() {
  while (kShellSurfaceCount > 0) {
    AbstractEventHandler *event_handler = kTop->GetEventHandler();
    delete event_handler;
  }
}

} // namespace gui
} // namespace wiztk
