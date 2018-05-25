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

#include "wiztk/gui/abstract-button.hpp"

#include "wiztk/base/clamp.hpp"
#include "wiztk/base/bit.hpp"

#include "wiztk/gui/key-event.hpp"
#include "wiztk/gui/mouse-event.hpp"
#include "wiztk/gui/theme.hpp"

#include "wiztk/graphics/font.hpp"

namespace wiztk {
namespace gui {

using base::RectF;
using base::Bit;

using graphics::Font;
using graphics::Typeface;

struct AbstractButton::Private {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);

  enum FlagIndex {

    /**! if this button is sensitive when mouse enter/leave (need to highlight when hover) */
        kSensitive = 0x1,

    /**! if the mouse is hovering on this button */
        kHovered = 0x1 << 1,

    /**! if the mouse is pressing this button */
        kPressed = 0x1 << 2,

    /**! if the mouse need to emit a click signal */
        kClicked = 0x1 << 3

  };

  Private() = default;
  ~Private() = default;

  uint32_t flags = 0x1;

  Font font = Theme::GetData().default_font;

  std::string text;

};

AbstractButton::AbstractButton()
    : AbstractButton(80, 20) {}

AbstractButton::AbstractButton(int width, int height)
    : AbstractView(width, height) {
  p_ = std::make_unique<Private>();
}

AbstractButton::AbstractButton(const std::string &text)
    : AbstractButton(80, 20) {
  p_->text = text;
}

AbstractButton::~AbstractButton() = default;

bool AbstractButton::IsSensitive() const {
  return (p_->flags & Private::FlagIndex::kSensitive) != 0;
}

bool AbstractButton::IsHovered() const {
  return (p_->flags & Private::FlagIndex::kHovered) != 0;
}

bool AbstractButton::IsPressed() const {
  return (p_->flags & Private::FlagIndex::kPressed) != 0;
}

const Font &AbstractButton::GetFont() const {
  return p_->font;
}

void AbstractButton::SetFont(const Font &font) {
  p_->font = font;
  Update();
}

const std::string &AbstractButton::GetText() const {
  return p_->text;
}

void AbstractButton::SetText(const std::string &text) {
  p_->text = text;
  Update();
}

void AbstractButton::OnMouseEnter(MouseEvent *event) {
  Bit::Set<uint32_t>(p_->flags, Private::kHovered);
  if (IsSensitive()) {
    Update();
  }
  event->Accept();
}

void AbstractButton::OnMouseLeave() {
  Bit::Clear<uint32_t>(p_->flags, Private::kHovered | Private::kClicked | Private::kPressed);
  if (IsSensitive()) {
    Update();
  }
}

void AbstractButton::OnMouseDown(MouseEvent *event) {
  Bit::Set<uint32_t>(p_->flags, Private::kPressed);
  Update();
  event->Accept();
}

void AbstractButton::OnMouseUp(MouseEvent *event) {
  if (IsPressed()) {
    Bit::Set<uint32_t>(p_->flags, Private::kClicked);
  }
  Bit::Clear<uint32_t>(p_->flags, Private::kPressed);
  Update();

  if (p_->flags & Private::kClicked) {
    Bit::Clear<uint32_t>(p_->flags, Private::kClicked);
    clicked_();
  }

  event->Accept();
}

void AbstractButton::OnMouseMove(MouseEvent *event) {
  event->Accept();
}

void AbstractButton::OnKeyDown(KeyEvent *event) {
  event->Accept();
}

void AbstractButton::OnKeyUp(KeyEvent *event) {
  event->Accept();
}

void AbstractButton::OnConfigureGeometry(const RectF &old_geometry, const RectF &new_geometry) {
  RequestSaveGeometry(new_geometry);
}

void AbstractButton::OnSaveGeometry(const RectF &old_geometry, const RectF &new_geometry) {
  SetBounds(0, 0, new_geometry.width(), new_geometry.height());
  Update();
}

void AbstractButton::SetSensitive(bool sensitive) {
  if (sensitive) {
    Bit::Set<uint32_t>(p_->flags, Private::kSensitive);
  } else {
    Bit::Clear<uint32_t>(p_->flags, Private::kSensitive);
  }
}

} // namespace gui
} // namespace wiztk
