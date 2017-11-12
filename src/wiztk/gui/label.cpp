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

#include "wiztk/gui/label.hpp"

#include "wiztk/base/memory.hpp"

#include "wiztk/graphic/font.hpp"
#include "wiztk/graphic/canvas.hpp"
#include "wiztk/graphic/paint.hpp"
#include "wiztk/graphic/text-box.hpp"

#include "wiztk/gui/context.hpp"
#include "wiztk/gui/key-event.hpp"
#include "wiztk/gui/mouse-event.hpp"

#include "wiztk/gui/theme.hpp"

namespace wiztk {
namespace gui {

using base::RectF;
using base::ColorF;
using graphic::Font;

struct Label::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private(const std::string &text)
      : text(text),
        foreground(0.2f, 0.2f, 0.2f),
        background(0.f, 0.f, 0.f, 0.f),
        font(Theme::GetData().default_font) {}

  ~Private() {}

  std::string text;
  base::ColorF foreground;
  base::ColorF background;
  Font font;

};

Label::Label(const std::string &text)
    : Label(200, 24, text) {
}

Label::Label(int width, int height, const std::string &text)
    : AbstractView(width, height) {
  p_ = std::make_unique<Private>(text);
}

Label::~Label() {

}

void Label::SetForeground(const ColorF &color) {
  if (p_->foreground != color) {
    p_->foreground = color;
    Update();
  }
}

void Label::SetBackground(const ColorF &color) {
  if (p_->background != color) {
    p_->background = color;
    Update();
  }
}

void Label::SetFont(const graphic::Font &font) {
  p_->font = font;
  Update();
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
  using graphic::Canvas;
  using graphic::Paint;
  using graphic::TextBox;

  int scale = context.surface()->GetScale();
  const RectF rect = GetBounds() * scale;

  Paint paint;
  paint.SetColor(p_->background);
  context.canvas()->DrawRect(rect, paint);

  paint.SetColor(p_->foreground);
  paint.SetAntiAlias(true);
  paint.SetStyle(Paint::kStyleFill);
  paint.SetFont(p_->font);
  paint.SetTextSize(p_->font.GetSize() * scale);

  float text_width = paint.MeasureText(p_->text.c_str(), p_->text.length());

  TextBox text_box;
  // Put the text at the center
  text_box.SetBox(rect.l + (rect.width() - text_width) / 2.f,
                  rect.t + 1.f, // move down a little for better look
                  rect.r - (rect.width() - text_width) / 2.f,
                  rect.b);
  text_box.SetSpacingAlign(TextBox::kSpacingAlignCenter);
  text_box.SetText(p_->text.c_str(), p_->text.length(), paint);
  text_box.Draw(*context.canvas());
}

} // namespace gui
} // namespace wiztk
