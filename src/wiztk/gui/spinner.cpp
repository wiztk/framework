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

#include "wiztk/gui/spinner.hpp"

#include "wiztk/base/property.hpp"
#include "wiztk/base/color.hpp"

#include "wiztk/gui/mouse-event.hpp"
#include "wiztk/gui/key-event.hpp"
#include "wiztk/gui/callback.hpp"
#include "wiztk/gui/context.hpp"

#include "wiztk/graphic/canvas.hpp"
#include "wiztk/graphic/paint.hpp"

#include "wiztk/numerical/clamp.hpp"

namespace wiztk {
namespace gui {

using base::ColorF;
using graphic::Canvas;
using graphic::Paint;

using namespace wiztk::numerical;

struct Spinner::Private : public base::Property<Spinner> {

  explicit Private(Spinner *owner)
      : base::Property<Spinner>(owner) {}

  ~Private() final = default;

  Callback frame_callback;

  ColorF foreground = 0xEFFFFFFF;
  ColorF background = 0xFF69DD69;

  float angle = 0.f;

  void Draw(const Context &context);

  void OnFrame(uint32_t serial);

};

void Spinner::Private::Draw(const Context &context) {
  angle += 5.f;
  if (angle > 360.f) angle = 0.f;

  Canvas *canvas = context.canvas();
  int scale = context.surface()->GetScale();
  canvas->Scale(scale, scale);

  const RectF &rect = owner()->GetBounds();
  float radius = ClampT(std::min(rect.width(), rect.height()) / 2.f - 50.f,
                        50.f, 200.f);

  Paint paint;
  paint.SetAntiAlias(true);

  paint.SetColor(background);
  paint.SetStyle(Paint::Style::kStyleFill);
  canvas->DrawRect(rect, paint);

  paint.SetColor(ColorF(foreground));
  paint.SetStyle(Paint::Style::kStyleStroke);
  paint.SetStrokeWidth(6.f);

  canvas->DrawArc(RectF(rect.center_x() - radius,
                        rect.center_y() - radius,
                        rect.center_x() + radius,
                        rect.center_y() + radius),
                  angle, 300.f, false, paint);

  frame_callback.Setup(*context.surface());
}

void Spinner::Private::OnFrame(uint32_t serial) {
  owner()->Update();
}

Spinner::Spinner() {
  p_ = std::make_unique<Private>(this);

  p_->frame_callback.done().Bind(p_.get(), &Private::OnFrame);
}

Spinner::~Spinner() {

}

void Spinner::OnConfigureGeometry(const RectF &old_rect, const RectF &new_rect) {
  RequestSaveGeometry(new_rect);
}

void Spinner::OnSaveGeometry(const RectF &old_rect, const RectF &new_rect) {
  SetBounds(0.f, 0.f, new_rect.width(), new_rect.height());
  Update();
}

void Spinner::OnMouseEnter(MouseEvent *event) {
  event->Accept();
}

void Spinner::OnMouseLeave() {

}

void Spinner::OnMouseMove(MouseEvent *event) {
  event->Ignore();
}

void Spinner::OnMouseDown(MouseEvent *event) {
  event->Ignore();
}

void Spinner::OnMouseUp(MouseEvent *event) {
  event->Ignore();
}

void Spinner::OnKeyDown(KeyEvent *event) {
  event->Ignore();
}

void Spinner::OnKeyUp(KeyEvent *event) {
  event->Ignore();
}

void Spinner::OnDraw(const Context &context) {
  p_->Draw(context);
}

}
}