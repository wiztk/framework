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

#include <wiztk/gui/video-view.hpp>
#include <wiztk/gui/surface.hpp>

#include <wiztk/gui/context.hpp>
#include <wiztk/graphics/canvas.hpp>
#include <wiztk/graphics/paint.hpp>

namespace wiztk {
namespace gui {

VideoView::VideoView()
    : VideoView(320, 240) {

}

VideoView::VideoView(int width, int height)
    : AbstractView(width, height),
      surface_(nullptr) {

}

VideoView::~VideoView() {
  delete surface_;
}

void VideoView::OnRequestUpdateFrom(AbstractView *view) {

}

Surface *VideoView::GetSurface(const AbstractView *view) const {
  return surface_;
}

void VideoView::OnSizeChanged(int width, int height) {

}

void VideoView::OnMouseEnter(MouseEvent *event) {

}

void VideoView::OnMouseLeave(MouseEvent *event) {

}

void VideoView::OnMouseMove(MouseEvent *event) {

}

void VideoView::OnMouseDown(MouseEvent *event) {

}

void VideoView::OnKeyDown(KeyEvent *event) {

}

void VideoView::OnDraw(const Context *context) {
  using graphics::Canvas;
  using graphics::Paint;

  Canvas *canvas = context->canvas();
  Paint paint;
  paint.SetColor(0xFF29FF29);
  canvas->DrawRect(GetBounds() * context->surface()->GetScale(), paint);
}

} // namespace gui
} // namespace wiztk
