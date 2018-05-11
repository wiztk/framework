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

#include "wiztk/gui/dialog.hpp"

#include "wiztk/gui/shared-memory-pool.hpp"
#include "wiztk/gui/title-bar.hpp"
#include "wiztk/gui/buffer.hpp"
#include <wiztk/gui/region.hpp>
#include "wiztk/gui/context.hpp"

#include <wiztk/graphics/paint.hpp>
#include "wiztk/graphics/canvas.hpp"
#include "wiztk/graphics/path.hpp"

namespace wiztk {
namespace gui {

using base::RectI;
using base::RectF;

using graphics::Canvas;
using graphics::Paint;
using graphics::Path;
using graphics::ClipOperation;

struct Dialog::Private {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);

  Private() = default;
  ~Private() = default;

  SharedMemoryPool pool;

  Buffer frame_buffer;

  TitleBar *title_bar = nullptr;

  AbstractView *content_view = nullptr;

};

Dialog::Dialog(const char *title, AbstractShellView *parent)
    : AbstractShellView(title, parent) {
  p_ = std::make_unique<Private>();
}

Dialog::~Dialog() {
  if (nullptr != p_->content_view) p_->content_view->Destroy();
  if (nullptr != p_->title_bar) p_->title_bar->Destroy();
}

void Dialog::OnShown() {
  ViewSurface *shell_surface = this->GetShellSurface();
  const Margin &margin = shell_surface->margin();

  // Create buffer and attach it to the shell surface:
  int width = GetWidth() + margin.lr();
  int height = GetHeight() + margin.tb();
  int32_t pool_size = width * 4 * height;

  p_->pool.Setup(pool_size);
  p_->frame_buffer.Setup(p_->pool, width, height,
                         width * 4, WL_SHM_FORMAT_ARGB8888);
  shell_surface->Attach(&p_->frame_buffer);
  shell_surface->Update();
}

void Dialog::OnConfigureSize(const Size &old_size, const Size &new_size) {
  RequestSaveSize(new_size);
}

void Dialog::OnSaveSize(const Size &old_size, const Size &new_size) {
  ViewSurface *shell_surface = this->GetShellSurface();
  const Margin &margin = shell_surface->margin();
  int width = new_size.width;
  int height = new_size.height;

  RectI input_rect(width, height);

  input_rect.left = margin.left - kResizingMargin.left;
  input_rect.top = margin.top - kResizingMargin.top;
  input_rect.Resize(width + kResizingMargin.lr(), height + kResizingMargin.tb());

  Region input_region;
  input_region.Add(input_rect.x(), input_rect.y(),
                   input_rect.width(), input_rect.height());
  shell_surface->SetInputRegion(input_region);

  // Reset buffer:
  width += margin.lr();
  height += margin.tb();

  int pool_size = width * 4 * height;
  p_->pool.Setup(pool_size);

  p_->frame_buffer.Setup(p_->pool, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
  shell_surface->Attach(&p_->frame_buffer);

  shell_surface->Update();
}

void Dialog::OnRenderSurface(ViewSurface *surface) {
  ViewSurface *shell_surface = GetShellSurface();
  const Margin &margin = shell_surface->margin();

  Canvas canvas((unsigned char *) p_->frame_buffer.GetData(),
                p_->frame_buffer.GetSize().width,
                p_->frame_buffer.GetSize().height);
  canvas.SetOrigin(margin.left, margin.top);
  DrawFrame(Context(shell_surface, &canvas));
  shell_surface->Damage(0, 0, GetWidth() + margin.lr(), GetHeight() + margin.tb());
  shell_surface->Commit();
}

void Dialog::DrawFrame(const Context &context) {
  Canvas *canvas = context.canvas();
  canvas->Clear();

  Path path;
  RectF geometry = RectF::FromXYWH(0.f, 0.f, GetWidth(), GetHeight());

  if ((!IsMaximized()) || (!IsFullscreen())) {
    // Drop shadow:
//    float radii[] = {
//        7.f, 7.f, // top-left
//        7.f, 7.f, // top-right
//        4.f, 4.f, // bottom-right
//        4.f, 4.f  // bottom-left
//    };
//    path.AddRoundRect(geometry, radii);
    path.AddRect(geometry);
    canvas->Save();
    canvas->ClipPath(path, ClipOperation::kClipDifference, true);
    DropShadow(context);
    canvas->Restore();
  } else {
    path.AddRect(geometry);
  }

//   Fill color:
  Paint paint;
  paint.SetAntiAlias(true);
  paint.SetColor(0xEFF0F0F0);
  canvas->DrawPath(path, paint);

  // Draw the client area:
//  paint.SetColor(0xEFE0E0E0);
//  canvas->Save();
//  canvas->ClipPath(path, kClipIntersect, true);
//  canvas->DrawRect(Rect::FromXYWH(0.f, 0.f, GetWidth(), GetHeight()), paint);
//  canvas->Restore();

  canvas->Flush();
}

} // namespace gui
} // namespace wiztk
