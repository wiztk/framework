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

#include <wiztk/gui/application.hpp>
#include <wiztk/gui/window.hpp>
#include <wiztk/gui/abstract-view.hpp>
#include <wiztk/gui/context.hpp>
#include <wiztk/gui/callback.hpp>
#include <wiztk/gui/key-event.hpp>
#include <wiztk/gui/mouse-event.hpp>

#include "wiztk/numerical/clamp.hpp"

#include <wiztk/graphic/canvas.hpp>
#include <wiztk/graphic/paint.hpp>

#include <iostream>

using namespace wiztk;
using namespace wiztk::base;
using namespace wiztk::gui;
using namespace wiztk::graphic;
using namespace wiztk::numerical;

class SpinningView : public AbstractView {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(SpinningView);

  SpinningView() {
    color_ = 0xFF69DD69;
    frame_callback_.done().Bind(this, &SpinningView::OnFrame);
  }

  ~SpinningView() override = default;

 protected:

  void OnConfigureGeometry(const RectF &old_geometry, const RectF &new_geometry) override {
    RequestSaveGeometry(new_geometry);
  }

  void OnSaveGeometry(const RectF &old_geometry, const RectF &new_geometry) override {
    Update();
  }

  void OnMouseEnter(MouseEvent *event) override {
    event->Accept();
  }

  void OnMouseLeave() override {

  }

  void OnMouseMove(MouseEvent *event) override {
    event->Ignore();
  }

  void OnMouseDown(MouseEvent *event) override {
    event->Ignore();
  }

  void OnMouseUp(MouseEvent *event) override {
    event->Ignore();
  }

  void OnKeyDown(KeyEvent *event) override {
    event->Ignore();
  }

  void OnKeyUp(KeyEvent *event) override {
    event->Ignore();
  }

  void OnDraw(const Context &context) override {
    angle_ += 5.f;
    if (angle_ > 360.f) angle_ = 0.f;

    frame_callback_.Setup(*context.surface());
    Animate(context);
  }

 private:

  void Animate(const Context &context) {
    static const int padding = 0;
    Canvas *canvas = context.canvas();
    canvas->Save();
    int scale = context.surface()->GetScale();
    canvas->Scale(scale, scale);

    const RectF &rect = GetGeometry();
    float radius_ = Clamp(std::min(rect.width(), rect.height()) / 2.f - 50.f,
                          50.f, 200.f);

    Paint paint;
    paint.SetAntiAlias(true);

    Canvas::LockGuard guard(context.canvas(), rect);

    paint.SetColor(color_);
    paint.SetStyle(Paint::Style::kStyleFill);
    canvas->DrawRect(RectF(rect.l + padding,
                           rect.t,
                           rect.r - padding,
                           rect.b - padding),
                     paint);

    paint.SetColor(ColorF(0xF7FFFFFF));
    paint.SetStyle(Paint::Style::kStyleStroke);
    paint.SetStrokeWidth(6.f);

    canvas->DrawArc(RectF(rect.center_x() - radius_,
                          rect.center_y() - radius_,
                          rect.center_x() + radius_,
                          rect.center_y() + radius_),
                    angle_, 300.f, false, paint);
    canvas->Restore();
  }

  void OnFrame(uint32_t serial) {
    Update();
  }

  Callback frame_callback_;
  ColorF color_;
  float angle_ = 0.f;
};

int main(int argc, char *argv[]) {
  Application app(argc, argv);

  auto *win = new Window(320, 280, "Simple Shm");
  win->SetAppId("Simple-Shm");

  auto *widget = new SpinningView;
  win->SetContentView(widget);

  win->Show();

  return app.Run();
}
