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
#include <wiztk/graphics/canvas.hpp>
#include <wiztk/graphics/paint.hpp>

#include <wiztk/gui/key-event.hpp>
#include <wiztk/gui/mouse-event.hpp>

#include "SkSurface.h"
#include "SkCanvas.h"

using namespace wiztk;
using namespace wiztk::base;
using namespace wiztk::gui;

class MainWidget : public AbstractView {

 public:

  MainWidget()
      : AbstractView() {
  }

  virtual ~MainWidget() {}

 protected:

  virtual void OnConfigureGeometry(const RectF &old_geometry, const RectF &new_geometry) override {
    RequestSaveGeometry(new_geometry);
  }

  virtual void OnSaveGeometry(const RectF &old_geometry, const RectF &new_geometry) override {
    Update();
  }

  virtual void OnMouseEnter(MouseEvent *event) override {
    event->Accept();
  }

  virtual void OnMouseLeave() override {

  }

  virtual void OnMouseMove(MouseEvent *event) override {
    event->Ignore();
  }

  virtual void OnMouseDown(MouseEvent *event) override {
    event->Ignore();
  }

  virtual void OnMouseUp(MouseEvent *event) override {
    event->Ignore();
  }

  virtual void OnKeyDown(KeyEvent *event) override {
    event->Ignore();
  }

  virtual void OnKeyUp(KeyEvent *event) override {
    event->Ignore();
  }

  void OnDraw(const Context &context) override {
    using wiztk::graphics::Canvas;
    using wiztk::graphics::Paint;

    const RectF &rect = GetBounds();
    int scale = context.surface()->GetScale();

    Canvas *canvas = context.canvas();
    canvas->Save();
    canvas->Scale(scale, scale);

    Paint paint;
    paint.SetColor(ColorF(0.855f, 0.855f, 0.165f, .9f));
    canvas->DrawRect(rect, paint);

    canvas->Restore();

    SkCanvas *sk_canvas = canvas->GetSkCanvas();

    SkImageInfo info = SkImageInfo::MakeN32(400, 400, kPremul_SkAlphaType);
    sk_sp<SkSurface> surface = sk_canvas->makeSurface(info);

    SkCanvas *top_canvas = surface->getCanvas();
    SkRect top_rect = SkRect::MakeXYWH(50, 50, 100, 100);
    SkPaint top_paint;
    top_paint.setColor(0xFFFF0000);

    top_canvas->drawRect(top_rect, top_paint);
    top_canvas->flush();

    surface->draw(sk_canvas, 100, 100, nullptr);

    canvas->Flush();
  }

};

int main(int argc, char *argv[]) {
  Application app(argc, argv);

  auto *win = new Window("Frameless Window");
//  win->SetAppId("Frameless-Demo");

//  auto *widget = new MainWidget;
//  win->SetContentView(widget);

  win->Show();

  return app.Run();
}
