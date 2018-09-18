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

#include "wiztk/gui/label.hpp"

#include "wiztk/graphics/font.hpp"
#include "wiztk/graphics/canvas.hpp"
#include "wiztk/graphics/paint.hpp"

#include "wiztk/gui/context.hpp"
#include "wiztk/gui/key-event.hpp"
#include "wiztk/gui/mouse-event.hpp"

#include "wiztk/gui/theme.hpp"

namespace wiztk {
namespace gui {

using base::RectF;
using base::ColorF;
using graphics::Font;

struct Label::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  explicit Private(const std::string &text)
      : text(text),
        fore_color(0.2f, 0.2f, 0.2f),
        back_color(0.f, 0.f, 0.f, 0.f),
        font(Theme::GetData().default_font) {}

  ~Private() = default;

  std::string text;
  ColorF fore_color;
  ColorF back_color;
  Font font;

};

Label::Label(const std::string &text)
    : Label(200, 24, text) {
}

Label::Label(int width, int height, const std::string &text)
    : AbstractView(width, height) {
  p_ = std::make_unique<Private>(text);
}

Label::~Label() = default;

void Label::SetForeColor(const ColorF &color) {
  if (p_->fore_color != color) {
    p_->fore_color = color;
    Update();
  }
}

const Label::ColorF &Label::GetForeColor() const {
  return p_->fore_color;
}

void Label::SetBackColor(const ColorF &color) {
  if (p_->back_color != color) {
    p_->back_color = color;
    Update();
  }
}

const Label::ColorF &Label::GetBackColor() const {
  return p_->back_color;
}

void Label::SetFont(const graphics::Font &font) {
  p_->font = font;
  Update();
}

const Label::Font &Label::GetFont() const {
  return p_->font;
}

void Label::OnConfigureGeometry(const RectF &old_geometry, const RectF &new_geometry) {
  RequestSaveGeometry(new_geometry);
}

void Label::OnSaveGeometry(const RectF &old_geometry, const RectF &new_geometry) {
  SetBounds(0.f, 0.f, new_geometry.width(), new_geometry.height());
  Update();
}

void Label::OnMouseEnter(MouseEvent *event) {
  event->Ignore();
}

void Label::OnMouseLeave() {

}

void Label::OnMouseMove(MouseEvent *event) {
  event->Ignore();
}

void Label::OnMouseDown(MouseEvent *event) {
  event->Ignore();
}

void Label::OnMouseUp(MouseEvent *event) {
  event->Ignore();
}

void Label::OnKeyDown(KeyEvent *event) {
  event->Ignore();
}

void Label::OnKeyUp(KeyEvent *event) {
  event->Ignore();
}

void Label::OnDraw(const Context &context) {
  using graphics::Canvas;
  using graphics::Paint;
  using graphics::TextAlignment;

  Canvas* const canvas = context.canvas();
  int scale = context.surface()->GetScale();
  const RectF rect = GetBounds() * scale;

  Paint paint;
  paint.SetColor(p_->back_color);
  canvas->DrawRect(rect, paint);

  paint.SetColor(p_->fore_color);
  paint.SetAntiAlias(true);
  paint.SetStyle(Paint::kStyleFill);
  paint.SetFont(p_->font);
  paint.SetTextSize(p_->font.GetSize() * scale);

  paint.SetTextAlign(TextAlignment::kCenter);
  canvas->DrawText(p_->text, rect.center_x(), rect.center_y(), paint, TextAlignment::kMiddle);
}

} // namespace gui
} // namespace wiztk
