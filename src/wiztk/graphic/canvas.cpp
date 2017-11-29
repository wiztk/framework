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

#include "internal/canvas_private.hpp"

#include "wiztk/base/memory.hpp"

#include <wiztk/graphic/paint.hpp>
#include <wiztk/graphic/path.hpp>

#include "internal/matrix_private.hpp"
#include "internal/surface_private.hpp"
#include "internal/image-info_private.hpp"

#include <unicode/utf.h>
#include <unicode/unistr.h>

namespace wiztk {
namespace graphic {

using base::Point2F;
using base::RectF;
using base::ColorF;

Canvas *Canvas::CreateRasterDirect(int width, int height, unsigned char *pixels, int format) {
  size_t stride = (size_t) width * 4;

  // TODO: support more pixel format
  switch (format) {
    case kPixelFormatABGR8888:
    default:break;
  }

  Bitmap bitmap;
  if (!bitmap.InstallPixels(ImageInfo::MakeN32Premul(width, height), pixels, stride)) {
    throw std::runtime_error("ERROR! Invalid bitmap format for Canvas!");
  }

  return new Canvas(bitmap);
}

Canvas::Canvas() {
  p_ = std::make_unique<Private>();

}

Canvas::Canvas(unsigned char *pixel, int width, int height, int format) {
  size_t stride = (size_t) width * 4;

  // TODO: support more pixel format
  switch (format) {
    case kPixelFormatABGR8888:
    default:break;
  }

  SkBitmap bitmap;
  if (!bitmap.installPixels(SkImageInfo::MakeN32Premul(width, height), pixel, stride)) {
    throw std::runtime_error("ERROR! Invalid bitmap format for Canvas!");
  }

  p_ = std::make_unique<Private>(bitmap);
}

Canvas::Canvas(const Bitmap &bitmap) {
  p_ = std::make_unique<Private>(bitmap.p_->sk_bitmap);
}

Canvas::~Canvas() {

}

void Canvas::SetOrigin(float x, float y) {
  p_->sk_canvas.translate(x - p_->origin.x, y - p_->origin.y);
  p_->origin.x = x;
  p_->origin.y = y;
}

ViewSurface *Canvas::CreateSurface(const ImageInfo &info) {
  sk_sp<SkSurface> native = p_->sk_canvas.makeSurface(SkImageInfo::Make(info.width(),
                                                                        info.height(),
                                                                        static_cast<SkColorType >(info.color_type()),
                                                                        static_cast<SkAlphaType >(info.alpha_type())));
  // TODO: create surface
  return nullptr;
}

void Canvas::DrawLine(float x0, float y0, float x1, float y1, const Paint &paint) {
  p_->sk_canvas.drawLine(x0, y0, x1, y1, paint.GetSkPaint());
}

void Canvas::DrawRect(const RectF &rect, const Paint &paint) {
  p_->sk_canvas.drawRect(*reinterpret_cast<const SkRect *>(&rect), paint.GetSkPaint());
}

void Canvas::DrawRoundRect(const RectF &rect, float rx, float ry, const Paint &paint) {
  p_->sk_canvas.drawRoundRect(*reinterpret_cast<const SkRect *>(&rect), rx, ry, paint.GetSkPaint());
}

void Canvas::DrawOval(const RectF &oval, const Paint &paint) {
  p_->sk_canvas.drawOval(*reinterpret_cast<const SkRect *>(&oval), paint.GetSkPaint());
}

void Canvas::DrawCircle(float x, float y, float radius, const Paint &paint) {
  p_->sk_canvas.drawCircle(x, y, radius, paint.GetSkPaint());
}

void Canvas::DrawArc(const RectF &oval, float start_angle, float sweep_angle, bool use_center, const Paint &paint) {
  p_->sk_canvas.drawArc(*reinterpret_cast<const SkRect *>(&oval),
                        start_angle,
                        sweep_angle,
                        use_center,
                        paint.GetSkPaint());
}

void Canvas::DrawPath(const Path &path, const Paint &paint) {
  p_->sk_canvas.drawPath(path.GetSkPath(), paint.GetSkPaint());
}

void Canvas::DrawText(const void *text, size_t byte_length, float x, float y, const Paint &paint) {
  p_->sk_canvas.drawText(text, byte_length, x, y, paint.GetSkPaint());
}

void Canvas::DrawText(const std::string &text, float x, float y, const Paint &paint) {
  p_->sk_canvas.drawText(text.data(), text.length(), x, y, paint.GetSkPaint());
}

void Canvas::DrawText(const String &text, float x, float y, const Paint &paint) {
  // TODO: This create and copy the string array from String16 to icu::UnicodeString, find a better way for performance.
  std::string utf8;
  icu::UnicodeString utf16(text.data(), static_cast<uint32_t>(text.length()));
  utf16.toUTF8String(utf8);

  p_->sk_canvas.drawText(utf8.data(), utf8.length(), x, y, paint.GetSkPaint());
}

void Canvas::DrawPaint(const Paint &paint) {
  p_->sk_canvas.drawPaint(paint.GetSkPaint());
}

void Canvas::Translate(float dx, float dy) {
  p_->sk_canvas.translate(dx, dy);
}

void Canvas::Scale(float sx, float sy) {
  p_->sk_canvas.scale(sx, sy);
}

void Canvas::Rotate(float degrees) {
  p_->sk_canvas.rotate(degrees);
}

void Canvas::Rotate(float degrees, float px, float py) {
  p_->sk_canvas.rotate(degrees, px, py);
}

void Canvas::Skew(float sx, float sy) {
  p_->sk_canvas.skew(sx, sy);
}

void Canvas::Concat(const Matrix &matrix) {
  p_->sk_canvas.concat(matrix.p_->sk_matrix);
}

void Canvas::SetMatrix(const Matrix &matrix) {
  p_->sk_canvas.setMatrix(matrix.p_->sk_matrix);
}

void Canvas::ResetMatrix() {
  p_->sk_canvas.resetMatrix();
  p_->sk_canvas.translate(p_->origin.x, p_->origin.y);
}

void Canvas::Clear(uint32_t argb) {
  p_->sk_canvas.clear(argb);
}

void Canvas::Clear(const ColorF &color) {
  p_->sk_canvas.clear(color.argb());
}

void Canvas::ClipRect(const RectF &rect, ClipOperation op, bool antialias) {
  p_->sk_canvas.clipRect(reinterpret_cast<const SkRect &>(rect), static_cast<SkClipOp >(op), antialias);
}

void Canvas::ClipRect(const RectF &rect, bool antialias) {
  p_->sk_canvas.clipRect(reinterpret_cast<const SkRect &>(rect), antialias);
}

void Canvas::ClipPath(const Path &path, ClipOperation op, bool antialias) {
  p_->sk_canvas.clipPath(path.GetSkPath(), static_cast<SkClipOp >(op), antialias);
}

void Canvas::ClipPath(const Path &path, bool antilias) {
  p_->sk_canvas.clipPath(path.GetSkPath(), antilias);
}

void Canvas::Save() {
  p_->sk_canvas.save();
}

void Canvas::SaveLayer(const RectF *bounds, const Paint *paint) {
  p_->sk_canvas.saveLayer(reinterpret_cast<const SkRect *>(bounds),
                          nullptr == paint ? nullptr : &paint->GetSkPaint());
}

void Canvas::SaveLayer(const RectF *bounds, unsigned char alpha) {
  p_->sk_canvas.saveLayerAlpha(reinterpret_cast<const SkRect *>(bounds), alpha);
}

void Canvas::Restore() {
  if (p_->lock_guard_deque.IsEmpty()) {
    p_->sk_canvas.restore();
    return;
  }

  if (p_->sk_canvas.getSaveCount() > p_->lock_guard_deque[-1]->depth) {
    p_->sk_canvas.restore();
  }
}

int Canvas::GetSaveCount() const {
  return p_->sk_canvas.getSaveCount();
}

void Canvas::RestoreToCount(int save_count) {
  if (p_->lock_guard_deque.IsEmpty()) {
    p_->sk_canvas.restoreToCount(save_count);
    return;
  }

  if (save_count > p_->lock_guard_deque[-1]->depth) {
    p_->sk_canvas.restoreToCount(save_count);
  }
}

void Canvas::Flush() {
  p_->sk_canvas.flush();
}

const Point2F &Canvas::GetOrigin() const {
  return p_->origin;
}

SkCanvas *Canvas::GetSkCanvas() const {
  return &p_->sk_canvas;
}

// ----------

Canvas::LockGuard::~LockGuard() {
  if (node_.IsLinked()) {
    base::DequeT<LockGuardNode>::Iterator it = canvas_->p_->lock_guard_deque.rbegin();
    while (it.element() != &node_) {
      it.Remove();
      it = canvas_->p_->lock_guard_deque.rbegin();
    }
    canvas_->p_->sk_canvas.restoreToCount(node_.depth);
  }
}

void Canvas::LockGuard::Lock(const RectF &rect, bool antilias) {
  if (node_.IsLinked()) return;

  canvas_->p_->lock_guard_deque.PushBack(&node_);
  node_.depth = canvas_->GetSaveCount();
  canvas_->Save();
  canvas_->ClipRect(rect, antilias);
}

void Canvas::LockGuard::Lock(const RectF &rect, ClipOperation op, bool antialias) {
  if (node_.IsLinked()) return;

  canvas_->p_->lock_guard_deque.PushBack(&node_);
  node_.depth = canvas_->GetSaveCount();
  canvas_->Save();
  canvas_->ClipRect(rect, op, antialias);
}

void Canvas::LockGuard::Lock(const Path &path, bool antialias) {
  if (node_.IsLinked()) return;

  canvas_->p_->lock_guard_deque.PushBack(&node_);
  node_.depth = canvas_->GetSaveCount();
  canvas_->Save();
  canvas_->ClipPath(path, antialias);
}

void Canvas::LockGuard::Lock(const Path &path, ClipOperation op, bool antialias) {
  if (node_.IsLinked()) return;

  canvas_->p_->lock_guard_deque.PushBack(&node_);
  node_.depth = canvas_->GetSaveCount();
  canvas_->Save();
  canvas_->ClipPath(path, op, antialias);
}

void Canvas::LockGuard::Lock(float dx, float dy) {
  if (node_.IsLinked()) return;

  canvas_->p_->lock_guard_deque.PushBack(&node_);
  node_.depth = canvas_->GetSaveCount();
  canvas_->Save();
  canvas_->Translate(dx, dy);
}

void Canvas::LockGuard::Unlock() {
  if (!node_.IsLinked()) return;

  base::DequeT<LockGuardNode>::Iterator it = canvas_->p_->lock_guard_deque.rbegin();
  while (it.element() != &node_) {
    it.Remove();
    it = canvas_->p_->lock_guard_deque.rbegin();
  }
  canvas_->p_->sk_canvas.restoreToCount(node_.depth);
  node_.Unlink();
}

} // namespace graphic
} // namespace wiztk
