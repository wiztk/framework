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

#include "abstract-shell-view_private.hpp"
#include "abstract-view_private.hpp"

#include "wiztk/base/bit.hpp"

#include "wiztk/async/event-loop.hpp"

#include "wiztk/gui/application.hpp"
#include "wiztk/gui/mouse-event.hpp"
#include "wiztk/gui/key-event.hpp"
#include "wiztk/gui/region.hpp"
#include "wiztk/gui/context.hpp"
#include "wiztk/gui/theme.hpp"

#include "wiztk/graphics/canvas.hpp"

#include "SkCanvas.h"
#include "SkImage.h"

namespace wiztk {
namespace gui {

using Point = base::Point2I;
using Size = base::SizeI;

using base::ThicknessI;
using base::SLOT;
using base::Bit;
using graphics::Canvas;

const AbstractShellView::Margin AbstractShellView::kResizingMargin(5, 5, 5, 5);

AbstractShellView::AbstractShellView(const char *title, AbstractShellView *parent)
    : AbstractShellView(400, 300, title, parent) {
}

AbstractShellView::AbstractShellView(int width,
                                     int height,
                                     const char *title,
                                     AbstractShellView *parent) {
  p_ = std::make_unique<Private>(this);
  __PROPERTY__(size).width = width;
  __PROPERTY__(size).height = height;
  __PROPERTY__(last_size) = __PROPERTY__(size);
  __PROPERTY__(parent) = parent;

  if (nullptr != title) __PROPERTY__(title) = title;

  if (nullptr == __PROPERTY__(parent)) {
    __PROPERTY__(shell_surface) = ViewSurface::Shell::Toplevel::Create(this, Theme::GetShadowMargin());
    auto *top_level = ViewSurface::Shell::Toplevel::Get(__PROPERTY__(shell_surface));
    top_level->SetTitle(title);
  } else {
    __PROPERTY__(shell_surface) =
        ViewSurface::Shell::Popup::Create(__PROPERTY__(parent)->__PROPERTY__(shell_surface),
                                          this,
                                          Theme::GetShadowMargin());
    // TODO: create popup shell surface
  }

  int x = 0, y = 0;  // The input region
  x += Theme::GetShadowMargin().left - kResizingMargin.left;
  y += Theme::GetShadowMargin().top - kResizingMargin.top;
  width += kResizingMargin.horizontal();
  height += kResizingMargin.vertical();

  Region input_region;
  input_region.Add(x, y, width, height);
  __PROPERTY__(shell_surface)->SetInputRegion(input_region);
}

AbstractShellView::~AbstractShellView() {
  delete __PROPERTY__(shell_surface);
}

void AbstractShellView::SetTitle(const char *title) {
  __PROPERTY__(title) = title;
  if (nullptr == __PROPERTY__(parent)) {
    ViewSurface::Shell::Toplevel::Get(__PROPERTY__(shell_surface))->SetTitle(title);
  }
}

const std::string &AbstractShellView::GetTitle() const {
  return __PROPERTY__(title);
}

void AbstractShellView::SetAppId(const char *app_id) {
  __PROPERTY__(app_id) = app_id;
  if (nullptr == __PROPERTY__(parent)) {
    ViewSurface::Shell::Toplevel::Get(__PROPERTY__(shell_surface))->SetAppId(app_id);
  }
}

void AbstractShellView::Show() {
  if (!IsShown()) {
    __PROPERTY__(shell_surface)->Commit();
    return;
  }
}

void AbstractShellView::Close(SLOT) {
  if (ViewSurface::CountShellSurfaces() == 1) {
    Application::GetInstance()->Exit();
  }

  // TODO: use a close task if there's more than 1 windows in an application

  // windows will be deleted when application exits, uncomment this line
  // sometimes cause segfault when close button is clicked:
  //  delete this;
}

void AbstractShellView::Minimize(SLOT) {
  auto *toplevel = ViewSurface::Shell::Toplevel::Get(__PROPERTY__(shell_surface));
  if (nullptr == toplevel) return;

  Bit::Set<int>(__PROPERTY__(flags), Private::kFlagMaskMinimized);
  toplevel->SetMinimized();
  _ASSERT(IsMinimized());
}

void AbstractShellView::ToggleMaximize(SLOT) {
  if (IsFullscreen()) return;

  auto *toplevel = ViewSurface::Shell::Toplevel::Get(__PROPERTY__(shell_surface));
  if (nullptr == toplevel) return;

  if (IsMaximized()) {
    toplevel->UnsetMaximized();
  } else {
    toplevel->SetMaximized();
  }
}

void AbstractShellView::ToggleFullscreen(const Output *output, SLOT) {
  auto *top_level = ViewSurface::Shell::Toplevel::Get(__PROPERTY__(shell_surface));
  if (nullptr == top_level) return;

  if (output) {
    top_level->SetFullscreen(output);
  } else {
    top_level->UnsetFullscreen();
  }
}

bool AbstractShellView::IsFullscreen() const {
  return 0 != (__PROPERTY__(flags) & Private::kFlagMaskFullscreen);
}

bool AbstractShellView::IsMaximized() const {
  return 0 != (__PROPERTY__(flags) & Private::kFlagMaskMaximized);
}

bool AbstractShellView::IsMinimized() const {
  return 0 != (__PROPERTY__(flags) & Private::kFlagMaskMinimized);
}

bool AbstractShellView::IsFocused() const {
  return 0 != (__PROPERTY__(flags) & Private::kFlagMaskFocused);
}

bool AbstractShellView::IsResizing() const {
  return 0 != (__PROPERTY__(flags) & Private::kFlagMaskResizing);
}

bool AbstractShellView::IsShown() const {
  return 0 != (__PROPERTY__(flags) & Private::kFlagMaskShown);
}

int AbstractShellView::GetWidth() const {
  return __PROPERTY__(size).width;
}

int AbstractShellView::GetHeight() const {
  return __PROPERTY__(size).height;
}

AbstractShellView *AbstractShellView::GetParent() const {
  return __PROPERTY__(parent);
}

void AbstractShellView::AttachView(AbstractView *view) {
  if (view->__PROPERTY__(shell_view) == this) {
    _ASSERT(nullptr == view->__PROPERTY__(parent));
    return;
  }

  if (nullptr != view->__PROPERTY__(parent)) {
    _ASSERT(nullptr == view->__PROPERTY__(shell_view));
    view->__PROPERTY__(parent)->RemoveChild(view);
    _ASSERT(nullptr == view->__PROPERTY__(parent));
    _ASSERT(nullptr == view->__PROPERTY__(previous));
    _ASSERT(nullptr == view->__PROPERTY__(next));
  } else if (nullptr != view->__PROPERTY__(shell_view)) {
    _ASSERT(nullptr == view->__PROPERTY__(parent));
    view->__PROPERTY__(shell_view)->DetachView(view);
  }

  view->__PROPERTY__(shell_view) = this;

  OnViewAttached(view);
  if (view->__PROPERTY__(shell_view) == this)
    view->OnAttachedToShellView();
}

void AbstractShellView::DetachView(AbstractView *view) {
  if (view->__PROPERTY__(shell_view) != this) return;

  _ASSERT(nullptr == view->__PROPERTY__(parent));
  view->__PROPERTY__(shell_view) = nullptr;

  OnViewDetached(view);
  if (view->__PROPERTY__(shell_view) != this)
    view->OnDetachedFromShellView(this);
}

void AbstractShellView::OnMouseEnter(MouseEvent *event) {
  // override in sub class
}

void AbstractShellView::OnMouseLeave() {
  // override in sub class
}

void AbstractShellView::OnMouseMove(MouseEvent *event) {
  // override in sub class
}

void AbstractShellView::OnMouseDown(MouseEvent *event) {
  // override in sub class
}

void AbstractShellView::OnMouseUp(MouseEvent *event) {
  // override in sub class
}

void AbstractShellView::OnKeyDown(KeyEvent *event) {
  // override in sub class
}

void AbstractShellView::OnKeyUp(KeyEvent *event) {
  // override in sub class
}

void AbstractShellView::OnRequestSaveGeometry(AbstractView *view) {
  async::Scheduler scheduler = async::EventLoop::GetCurrent()->GetScheduler();

  if (__PROPERTY__(geometry_task).IsQueued()) {
    scheduler.PostMessageAfter(&__PROPERTY__(geometry_task),
                               AbstractView::GeometryMessage::Get(view));
    return;
  }

  scheduler.PostMessage(AbstractView::GeometryMessage::Get(view));
}

void AbstractShellView::OnRequestUpdateFrom(AbstractView *view) {
  // override in sub class
}

void AbstractShellView::OnEnterOutput(const ViewSurface *surface, const Output *output) {
}

void AbstractShellView::OnLeaveOutput(const ViewSurface *surface, const Output *output) {
}

//void AbstractShellView::OnDraw(const Context *context) {
// override in sub class
//}

void AbstractShellView::OnMaximized(bool maximized) {

}

void AbstractShellView::OnFullscreen(bool fullscreened) {

}

void AbstractShellView::OnFocus(bool focus) {
  // override in sub class
}

void AbstractShellView::OnViewAttached(AbstractView *view) {

}

void AbstractShellView::OnViewDetached(AbstractView *view) {

}

bool AbstractShellView::RequestSaveSize(const Size &size) {
  __PROPERTY__(size) = size;

  if (__PROPERTY__(last_size) == __PROPERTY__(size)) {
    __PROPERTY__(geometry_task).Unlink();
    return false;
  }

  if (!__PROPERTY__(geometry_task).IsQueued()) {
    async::EventLoop::GetCurrent()->GetScheduler().PostMessage(&__PROPERTY__(geometry_task));
  }

  return true;
}

void AbstractShellView::MoveWithMouse(MouseEvent *event) const {
  ViewSurface::Shell::Toplevel::Get(__PROPERTY__(shell_surface))->Move(*event, event->GetSerial());
}

void AbstractShellView::ResizeWithMouse(MouseEvent *event, uint32_t edges) const {
  ViewSurface::Shell::Toplevel::Get(__PROPERTY__(shell_surface))->Resize(*event, event->GetSerial(), edges);
}

ViewSurface *AbstractShellView::GetShellSurface() const {
  return __PROPERTY__(shell_surface);
}

void AbstractShellView::DispatchUpdate(AbstractView *view) {
  view->Update();
  view->DispatchUpdate();
}

void AbstractShellView::Draw(AbstractView *view, const Context &context) {
  using base::RectF;

  const RectF &geometry = view->GetGeometry();
  const RectF &bounds = view->GetBounds();
  int scale = context.surface()->GetScale();

  float x = geometry.left + bounds.left;
  float y = geometry.top + bounds.top;

  // Translate and lock the status:
  Canvas::LockGuard guard(context.canvas(), x * scale, y * scale);

  view->OnDraw(context);
}

void AbstractShellView::DispatchMouseEnterEvent(AbstractView *view, MouseEvent *event) {
  Point cursor = event->GetWindowXY();
  MouseTask *mouse_task = MouseTask::Get(this);

  if (nullptr == mouse_task->next()) {
    _ASSERT(mouse_task->event_handler() == this);
    _ASSERT(nullptr == mouse_task->previous());
    if (view->Contain(cursor.x, cursor.y)) {
      view->OnMouseEnter(event);
      if (event->IsAccepted()) {
        mouse_task->push_back(MouseTask::Get(view));
        mouse_task = mouse_task->next();
        __PROPERTY__(DispatchMouseEnterEvent)(view, event, mouse_task);
      } else if (event->IsIgnored()) {
        __PROPERTY__(DispatchMouseEnterEvent)(view, event, mouse_task);
      }
    }
  } else {
    while (mouse_task->next()) mouse_task = mouse_task->next(); // move to tail
    AbstractView *last = nullptr;
    MouseTask *tail = nullptr;
    while (mouse_task->previous()) {
      tail = mouse_task;
      last = static_cast<AbstractView *>(tail->event_handler());
      if (last->Contain(cursor.x, cursor.y)) {
        break;
      }
      mouse_task = mouse_task->previous();
      tail->unlink();
      last->OnMouseLeave();
      if (nullptr == mouse_task->previous()) break;
    }

    __PROPERTY__(DispatchMouseEnterEvent)(last, event, mouse_task);
  }
}

void AbstractShellView::DispatchMouseLeaveEvent() {
  MouseTask *it = MouseTask::Get(this)->next();

  MouseTask *tmp = nullptr;
  while (it) {
    tmp = it;
    it = it->next();
    tmp->unlink();
    tmp->event_handler()->OnMouseLeave();
  }
}

void AbstractShellView::DispatchMouseDownEvent(MouseEvent *event) {
  _ASSERT(event->GetState() == kMouseButtonPressed);

  MouseTask *it = MouseTask::Get(this)->next();
  while (it) {
    it->event_handler()->OnMouseDown(event);
    if (event->IsRejected()) break;
    it = it->next();
  }
}

void AbstractShellView::DispatchMouseUpEvent(MouseEvent *event) {
  _ASSERT(event->GetState() == kMouseButtonReleased);

  MouseTask *it = MouseTask::Get(this)->next();
  while (it) {
    it->event_handler()->OnMouseUp(event);
    if (event->IsRejected()) break;
    it = it->next();
  }
}

void AbstractShellView::DropShadow(const Context &context) {
  int scale = context.surface()->GetScale();
  float rad = (Theme::GetShadowRadius() - 1.f); // The spread radius
  float offset_x = Theme::GetShadowOffsetX();
  float offset_y = Theme::GetShadowOffsetY();

  int width = GetWidth();
  int height = GetHeight();

  if (!IsFocused()) {
    rad = (int) rad / 3;
    offset_x = (int) offset_x / 3;
    offset_y = (int) offset_y / 3;
  }

  // shadow map
  SkCanvas *c = context.canvas()->GetSkCanvas();
  c->save();
  c->scale(scale, scale);
  sk_sp<SkImage> image = SkImage::MakeFromRaster(*Theme::GetShadowPixmap(), nullptr, nullptr);

  // top-left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 0,
                                    2 * Theme::GetShadowRadius(), 2 * Theme::GetShadowRadius()),
                   SkRect::MakeXYWH(-rad + offset_x, -rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // top
  c->drawImageRect(image,
                   SkRect::MakeLTRB(2 * Theme::GetShadowRadius(), 0,
                                    250 - 2 * Theme::GetShadowRadius(), 2 * Theme::GetShadowRadius()),
                   SkRect::MakeXYWH(rad + offset_x, -rad + offset_y,
                                    width - 2 * rad, 2 * rad),
                   nullptr);

  // top-right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::GetShadowRadius(), 0,
                                    250, 2 * Theme::GetShadowRadius()),
                   SkRect::MakeXYWH(width - rad + offset_x, -rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 2 * Theme::GetShadowRadius(),
                                    2 * Theme::GetShadowRadius(), 250 - 2 * Theme::GetShadowRadius()),
                   SkRect::MakeXYWH(-rad + offset_x, rad + offset_y,
                                    2 * rad, height - 2 * rad),
                   nullptr);

  // bottom-left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 250 - 2 * Theme::GetShadowRadius(),
                                    2 * Theme::GetShadowRadius(), 250),
                   SkRect::MakeXYWH(-rad + offset_x, height - rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // bottom
  c->drawImageRect(image,
                   SkRect::MakeLTRB(2 * Theme::GetShadowRadius(), 250 - 2 * Theme::GetShadowRadius(),
                                    250 - 2 * Theme::GetShadowRadius(), 250),
                   SkRect::MakeXYWH(rad + offset_x, height - rad + offset_y,
                                    width - 2 * rad, 2 * rad),
                   nullptr);

  // bottom-right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::GetShadowRadius(), 250 - 2 * Theme::GetShadowRadius(),
                                    250, 250),
                   SkRect::MakeXYWH(width - rad + offset_x,
                                    height - rad + offset_y,
                                    2 * rad,
                                    2 * rad),
                   nullptr);

  // right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::GetShadowRadius(), 2 * Theme::GetShadowRadius(),
                                    250, 250 - 2 * Theme::GetShadowRadius()),
                   SkRect::MakeXYWH(width - rad + offset_x, rad + offset_y,
                                    2 * rad, height - 2 * rad),
                   nullptr);
  c->restore();
}

// ---------

void AbstractShellView::GeometryMessage::Exec() {
  shell_view_->OnSaveSize(shell_view_->__PROPERTY__(last_size), shell_view_->__PROPERTY__(size));
  shell_view_->__PROPERTY__(last_size) = shell_view_->__PROPERTY__(size);
  ViewSurface::Shell::Get(shell_view_->__PROPERTY__(shell_surface))->ResizeWindow(shell_view_->__PROPERTY__(size).width,
                                                                                  shell_view_->__PROPERTY__(size).height);  // Call xdg surface api
}

} // namespace gui
} // namespace wiztk
