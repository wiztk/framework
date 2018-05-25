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

#include "wiztk/gui/title-bar.hpp"

#include "wiztk/gui/mouse-event.hpp"
#include "wiztk/gui/key-event.hpp"
#include "wiztk/gui/context.hpp"
#include "wiztk/gui/theme.hpp"

#include "wiztk/graphics/canvas.hpp"
#include "wiztk/graphics/paint.hpp"
#include "wiztk/graphics/path.hpp"
#include "wiztk/graphics/gradient-shader.hpp"

#include "SkCanvas.h"
//#include "SkTypeface.h"
//#include "SkPaint.h"
#include "SkTextBox.h"

namespace wiztk {
namespace gui {

using base::RectF;
using graphics::Canvas;
using graphics::Paint;
using graphics::Path;

class TitleBar::Button : public AbstractButton {
 public:

  Button();

  ~Button() override = default;

  static const int kButtonSize = 14;

 protected:

  void OnDraw(const Context &context) override;

};

TitleBar::Button::Button()
    : AbstractButton(kButtonSize, kButtonSize) {
}

void TitleBar::Button::OnDraw(const Context &context) {
  // override in sub class
}

class TitleBar::CloseButton : public TitleBar::Button {

 public:

  CloseButton() {
    SetName("Close Button");
  }

  ~CloseButton() final = default;

 protected:

  void OnDraw(const Context &context) final;

};

void TitleBar::CloseButton::OnDraw(const Context &context) {
  Canvas *canvas = context.canvas();
  int scale = context.surface()->GetScale();
  const RectF &rect = GetBounds();

  canvas->Scale(scale, scale);

  Paint paint;
  paint.SetAntiAlias(true);
  base::ColorF background = 0xFFDD6666;

  if (IsHovered()) {
    if (IsPressed()) {
      background -= 45;
    } else {
      background += 35;
    }
  }

  paint.SetStyle(Paint::Style::kStyleFill);
  paint.SetColor(background);
  canvas->DrawCircle(rect.center_x(), rect.center_y(), 7.f, paint);

//  paint.SetColor(Theme::GetData().title_bar.active.foreground.color);
  paint.SetColor(Theme::GetData().title_bar.active.background);
  paint.SetStrokeWidth(1.5f);
  canvas->DrawLine(rect.center_x() - 3.f, rect.center_y() - 3.f,
                   rect.center_x() + 3.f, rect.center_y() + 3.f,
                   paint);
  canvas->DrawLine(rect.center_x() + 3.f, rect.center_y() - 3.f,
                   rect.center_x() - 3.f, rect.center_y() + 3.f,
                   paint);
}

class TitleBar::MaximizeButton : public TitleBar::Button {

 public:

  MaximizeButton() {
    SetName("Maximize Button");
  }

  ~MaximizeButton() final = default;

 protected:

  void OnDraw(const Context &context) final;

};

void TitleBar::MaximizeButton::OnDraw(const Context &context) {
  Canvas *canvas = context.canvas();
  int scale = context.surface()->GetScale();
  const RectF &rect = GetBounds();

  canvas->Scale(scale, scale);

  Paint paint;
  paint.SetAntiAlias(true);

  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(Theme::GetData().title_bar.highlight.foreground);
      paint.SetStyle(Paint::Style::kStyleFill);
      canvas->DrawCircle(rect.center_x(), rect.center_y(), 7.f, paint);
      paint.Reset();
      paint.SetAntiAlias(true);
    }

    paint.SetStyle(Paint::Style::kStyleStroke);
    paint.SetColor(Theme::GetData().title_bar.active.foreground);
    paint.SetStrokeWidth(1.f);
    canvas->DrawCircle(rect.center_x(), rect.center_y(), 6.5f, paint);
  }

  paint.SetColor(Theme::GetData().title_bar.active.foreground);
  paint.SetStrokeWidth(1.5f);
  canvas->DrawLine(rect.center_x() - 4.f, rect.center_y(),
                   rect.center_x() + 4.f, rect.center_y(),
                   paint);
  canvas->DrawLine(rect.center_x(), rect.center_y() - 4.f,
                   rect.center_x(), rect.center_y() + 4.f,
                   paint);
}

class TitleBar::MinimizeButton : public TitleBar::Button {

 public:

  MinimizeButton() {
    SetName("Minimize Button");
  }

  ~MinimizeButton() final = default;

 protected:

  void OnDraw(const Context &context) final;

};

void TitleBar::MinimizeButton::OnDraw(const Context &context) {
  Canvas *canvas = context.canvas();
  int scale = context.surface()->GetScale();
  const RectF &rect = GetBounds();

  canvas->Scale(scale, scale);

  Paint paint;
  paint.SetAntiAlias(true);

  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(Theme::GetData().title_bar.highlight.foreground);
      paint.SetStyle(Paint::Style::kStyleFill);
      canvas->DrawCircle(rect.center_x(), rect.center_y(), 7.f, paint);
      paint.Reset();
      paint.SetAntiAlias(true);
    }

    paint.SetStyle(Paint::Style::kStyleStroke);
    paint.SetColor(Theme::GetData().title_bar.active.foreground);
    paint.SetStrokeWidth(1.f);
    canvas->DrawCircle(rect.center_x(), rect.center_y(), 6.5f, paint);
  }

  paint.SetColor(Theme::GetData().title_bar.active.foreground);
  paint.SetStrokeWidth(1.5f);
  canvas->DrawLine(rect.center_x() - 4.f, rect.center_y(),
                   rect.center_x() + 4.f, rect.center_y(),
                   paint);
}

class TitleBar::FullscreenButton : public TitleBar::Button {

 public:

  FullscreenButton() {
    SetName("Fullscreen Button");
  }

  ~FullscreenButton() final = default;

 protected:

  void OnDraw(const Context &context) final;
};

void TitleBar::FullscreenButton::OnDraw(const Context &context) {
  Canvas *canvas = context.canvas();
  int scale = context.surface()->GetScale();
  const RectF &rect = GetBounds();

  canvas->Scale(scale, scale);

  Paint paint;
  paint.SetAntiAlias(true);

  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(Theme::GetData().title_bar.active.foreground);
      paint.SetStyle(Paint::Style::kStyleFill);
      canvas->DrawCircle(rect.center_x(), rect.center_y(), 7.f, paint);
      paint.Reset();
      paint.SetAntiAlias(true);
    }

    paint.SetStyle(Paint::Style::kStyleStroke);
    paint.SetColor(Theme::GetData().title_bar.active.foreground);
    paint.SetStrokeWidth(1.f);
    canvas->DrawCircle(rect.center_x(), rect.center_y(), 6.5f, paint);
  }

  paint.SetColor(Theme::GetData().title_bar.active.foreground);
  paint.SetStyle(Paint::Style::kStyleFill);

  Path path;
  path.MoveTo(-5.f, 0.f);
  path.RelativeLineTo(3.5f, -3.5f);
  path.RelativeLineTo(0.f, 7.f);
  path.Close();

  canvas->Translate(rect.center_x(), rect.center_y());
  canvas->Rotate(-45.f);
  canvas->DrawPath(path, paint);
  canvas->Rotate(180.f);
  canvas->DrawPath(path, paint);
}

TitleBar::TitleBar()
    : AbstractView(),
      close_button_(nullptr),
      maximize_button_(nullptr),
      minimize_button_(nullptr),
      fullscreen_button_(nullptr),
      font_(Theme::GetData().title_bar_font) {
  close_button_ = new CloseButton;
  maximize_button_ = new MaximizeButton;
  minimize_button_ = new MinimizeButton;
  fullscreen_button_ = new FullscreenButton;

  PushBackChild(fullscreen_button_);
  PushBackChild(minimize_button_);
  PushBackChild(maximize_button_);
  PushBackChild(close_button_);

  SetName("Title Bar");
}

void TitleBar::SetTitle(const std::string &title) {
  title_ = title;
  Update();
}

AbstractButton *TitleBar::GetButton(ButtonType button_type) const {
  switch (button_type) {
    case kButtonClose: return close_button_;
    case kButtonMaximize: return maximize_button_;
    case kButtonMinimize: return minimize_button_;
    case kButtonFullscreen: return fullscreen_button_;
    default: return nullptr;
  }
}

void TitleBar::OnConfigureGeometry(const RectF &old_geometry, const RectF &new_geometry) {
  RequestSaveGeometry(new_geometry);
  SetBounds(0.f, 0.f, new_geometry.width(), new_geometry.height());
}

void TitleBar::OnSaveGeometry(const RectF &old_geometry, const RectF &new_geometry) {
  SetBounds(0.f, 0.f, new_geometry.width(), new_geometry.height());

  int y = ((int) new_geometry.height() - close_button_->GetHeight()) / 2;
  int x = y;
  close_button_->MoveTo(x, y);

//  new_x += close_button_->GetWidth() + kButtonSpace;
//  maximize_button_->MoveTo(new_x, new_y);
//
//  new_x += maximize_button_->GetWidth() + kButtonSpace;
//  minimize_button_->MoveTo(new_x, new_y);

  x = (int) new_geometry.width() - y - fullscreen_button_->GetWidth();
  fullscreen_button_->MoveTo(x, y);

  x -= maximize_button_->GetWidth() + kButtonSpace;
  maximize_button_->MoveTo(x, y);

  x -= minimize_button_->GetWidth() + kButtonSpace;
  minimize_button_->MoveTo(x, y);
}

void TitleBar::OnMouseEnter(MouseEvent *event) {
  event->Ignore();
}

void TitleBar::OnMouseLeave() {

}

void TitleBar::OnMouseMove(MouseEvent *event) {
  event->Accept();
}

void TitleBar::OnMouseDown(MouseEvent *event) {
  event->Accept();
}

void TitleBar::OnMouseUp(MouseEvent *event) {
  event->Accept();
}

void TitleBar::OnKeyDown(KeyEvent *event) {
  event->Accept();
}

void TitleBar::OnKeyUp(KeyEvent *event) {
  event->Accept();
}

void TitleBar::OnDraw(const Context &context) {
  using namespace base;
  using namespace graphics;

  int scale = context.surface()->GetScale();

  const RectF bounds = GetBounds() * scale;
  float factor = 0.5f * scale;

  Paint paint;
  paint.SetAntiAlias(true);
  paint.SetStyle(Paint::kStyleFill);

  Point2F points[2] = {{factor, bounds.top + factor}, {factor, bounds.bottom}};

  uint32_t color1 = (Theme::GetData().title_bar.active.background + 5).argb();
  uint32_t color2 = (Theme::GetData().title_bar.active.background - 15).argb();
  uint32_t colors[2] = {color1, color2};
  float pos[2] = {0.f, 1.f};

  Shader shader = GradientShader::MakeLinear(points, colors, pos, 2, Shader::kTileModeClamp);
  paint.SetShader(shader);

  context.canvas()->DrawRect(RectF::FromLTRB(bounds.left + factor,
                                             bounds.top + factor,
                                             bounds.right - factor,
                                             bounds.bottom), paint);

  paint.Reset();
  paint.SetAntiAlias(true);
  paint.SetStyle(Paint::kStyleFill);
  paint.SetFont(font_);
  paint.SetTextSize(font_.GetSize() * scale);

  paint.SetColor(Theme::GetData().title_bar.active.foreground);

  float text_width = paint.MeasureText(title_.c_str(), title_.length());

  SkTextBox text_box;
  // Put the foreground at the center
  text_box.setBox(bounds.l + (bounds.width() - text_width) / 2.f,
                  bounds.t + 1.f, // move down a little for better look
                  bounds.r - (bounds.width() - text_width) / 2.f,
                  bounds.b);
  text_box.setSpacingAlign(SkTextBox::kCenter_SpacingAlign);
  text_box.setText(title_.c_str(), title_.length(), paint.GetSkPaint());
  text_box.draw(context.canvas()->GetSkCanvas());

}

} // namespace gui
} // namespace wiztk
