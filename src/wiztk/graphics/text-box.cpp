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

#include <wiztk/graphics/text-box.hpp>

#include <wiztk/graphics/canvas.hpp>
#include <wiztk/graphics/paint.hpp>

#include "SkTextBox.h"

namespace wiztk {
namespace graphics {

using base::RectF;

struct TextBox::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private() {}

  ~Private() {}

  SkTextBox sk_text_box;

};

TextBox::TextBox() {
  p_.reset(new Private);
}

TextBox::~TextBox() {

}

TextBox::Mode TextBox::GetMode() const {
  return static_cast<Mode>(p_->sk_text_box.getMode());
}

void TextBox::SetMode(Mode mode) {
  p_->sk_text_box.setMode(static_cast<SkTextBox::Mode>(mode));
}

TextBox::SpacingAlign TextBox::GetSpacingAlign() const {
  return static_cast<SpacingAlign >(p_->sk_text_box.getSpacingAlign());
}

void TextBox::SetSpacingAlign(SpacingAlign align) {
  p_->sk_text_box.setSpacingAlign(static_cast<SkTextBox::SpacingAlign >(align));
}

void TextBox::GetBox(RectF *rect) const {
  p_->sk_text_box.getBox(reinterpret_cast<SkRect *>(rect));
}

void TextBox::SetBox(const RectF &rect) {
  p_->sk_text_box.setBox(reinterpret_cast<const SkRect &>(rect));
}

void TextBox::SetBox(float left, float top, float right, float bottom) {
  p_->sk_text_box.setBox(left, top, right, bottom);
}

void TextBox::GetSpacing(float *mul, float *add) const {
  p_->sk_text_box.getSpacing(mul, add);
}

void TextBox::SetSpacing(float mul, float add) {
  p_->sk_text_box.setSpacing(mul, add);
}

void TextBox::Draw(const Canvas &canvas, const char *text, size_t len, const Paint &paint) {
  p_->sk_text_box.draw(canvas.GetSkCanvas(), text, len, paint.GetSkPaint());
}

void TextBox::SetText(const char *text, size_t len, const Paint &paint) {
  p_->sk_text_box.setText(text, len, paint.GetSkPaint());
}

void TextBox::Draw(const Canvas &canvas) {
  p_->sk_text_box.draw(canvas.GetSkCanvas());
}

int TextBox::CountLines() const {
  return p_->sk_text_box.countLines();
}

float TextBox::GetTextHeight() const {
  return p_->sk_text_box.getTextHeight();
}

} // namespace graphics
} // namespace wiztk
