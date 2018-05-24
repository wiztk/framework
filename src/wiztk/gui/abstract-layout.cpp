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

#include <wiztk/gui/abstract-layout.hpp>

#include <wiztk/gui/mouse-event.hpp>
#include <wiztk/gui/key-event.hpp>

#include <wiztk/graphics/canvas.hpp>
#include <wiztk/graphics/paint.hpp>

#include "abstract-view/iterators.hpp"

//#ifdef DEBUG
#include <cstdlib>
#include <wiztk/gui/timer.hpp>
#include <wiztk/base/macros.hpp>
#include "wiztk/gui/context.hpp"
//#endif

namespace wiztk {
namespace gui {

using base::RectF;
using base::ColorF;

using graphics::Paint;
using graphics::Canvas;

AbstractLayout::AbstractLayout(const Padding &padding)
    : is_geometry_saved_(false) {
  p_->padding = padding;
}

AbstractLayout::~AbstractLayout() = default;

void AbstractLayout::AddView(AbstractView *view) {
  _ASSERT(view->p_->parent == view->p_->layout);

  if (view->p_->layout == this) return;

  if (view->p_->layout) view->p_->layout->RemoveView(view);

  _ASSERT(nullptr == view->p_->layout);
  _ASSERT(nullptr == view->p_->parent);

  InsertChild(view);
  Layout();
}

void AbstractLayout::RemoveView(AbstractView *view) {
  _ASSERT(view->p_->parent == view->p_->layout);

  if (view->p_->layout != this) return;

  RemoveChild(view);
  Layout();
}

void AbstractLayout::Layout() {
  OnLayout(0, 0, 0, 0);
}

void AbstractLayout::OnConfigureGeometry(const RectF &old_geometry, const RectF &new_geometry) {
  if (!RequestSaveGeometry(new_geometry)) return;

  OnLayout(0, 0, 0, 0);
}

void AbstractLayout::OnSaveGeometry(const RectF &old_geometry, const RectF &new_geometry) {
  // Do nothing in layout
}

void AbstractLayout::OnRequestUpdateFrom(AbstractView *view) {
  if (view == this) return; // This layout does not need to update

  AbstractView::OnRequestUpdateFrom(view);
}

void AbstractLayout::OnChildAdded(AbstractView *view) {
  view->p_->layout = this;
  OnViewAdded(view);
}

void AbstractLayout::OnChildRemoved(AbstractView *view) {
  view->p_->layout = nullptr;
  OnViewRemoved(view);
}

void AbstractLayout::OnMouseEnter(MouseEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnMouseLeave() {

}

void AbstractLayout::OnMouseMove(MouseEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnMouseDown(MouseEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnMouseUp(MouseEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnKeyDown(KeyEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnKeyUp(KeyEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnDraw(const Context &context) {
  //#ifdef DEBUG
  srand(Timer::GetClockTime());
  float r = rand() % 255 / 255.f;
  float g = rand() % 255 / 255.f;
  float b = rand() % 255 / 255.f;

  Paint paint;
  paint.SetColor(ColorF(r, g, b, 0.25));
  context.canvas()->DrawRect(GetBounds() * context.surface()->GetScale(), paint);
  //#endif
}

} // namespace gui
} // namespace wiztk
