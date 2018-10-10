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

#include "path/private.hpp"

namespace wiztk {
namespace graphics {

using base::Point2F;
using base::RectF;

Path::Path() {
  p_.reset(new Private);
}

Path::Path(const Path &other) {
  p_.reset(new Private(*other.p_));
}

Path::~Path() {
}

Path &Path::operator=(const Path &other) {
  *p_ = *other.p_;
  return *this;
}

bool Path::IsInterpolatable(const Path &compare) const {
  return p_->sk_path.isInterpolatable(compare.p_->sk_path);
}

bool Path::Interpolate(const Path &ending, float weight, Path *out) const {
  return p_->sk_path.interpolate(ending.p_->sk_path, weight, &out->p_->sk_path);
}

void Path::AddRect(const RectF &rect, Direction dir) {
  p_->sk_path.addRect(reinterpret_cast<const SkRect &>(rect), static_cast<SkPath::Direction >(dir));
}

void Path::AddRect(const RectF &rect, Direction dir, unsigned start) {
  p_->sk_path.addRect(reinterpret_cast<const SkRect &>(rect), static_cast<SkPath::Direction >(dir), start);
}

void Path::AddRect(float left, float top, float right, float bottom, Direction dir) {
  p_->sk_path.addRect(left, top, right, bottom, static_cast<SkPath::Direction >(dir));
}

void Path::AddRoundRect(const RectF &rect, const float radii[], Direction dir) {
  p_->sk_path.addRoundRect(reinterpret_cast<const SkRect &>(rect),
                           radii, static_cast<SkPath::Direction>(dir));
}

Path::FillType Path::GetFillType() const {
  return (FillType) p_->sk_path.getFillType();
}

void Path::SetFillType(FillType fill_type) {
  p_->sk_path.setFillType((SkPath::FillType) fill_type);
}

bool Path::IsInverseFillType() const {
  return p_->sk_path.isInverseFillType();
}

void Path::ToggleInverseFillType() {
  p_->sk_path.toggleInverseFillType();
}

Path::Convexity Path::GetConvexity() const {
  return (Convexity) p_->sk_path.getConvexity();
}

Path::Convexity Path::GetConvexityOrUnknown() const {
  return (Convexity) p_->sk_path.getConvexityOrUnknown();
}

void Path::SetConvexity(Convexity convexity) {
  p_->sk_path.setConvexity((SkPath::Convexity) convexity);
}

bool Path::IsConvex() const {
  return p_->sk_path.isConvex();
}

void Path::Reset() {
  p_->sk_path.reset();
}

void Path::Rewind() {
  p_->sk_path.rewind();
}

bool Path::IsEmpty() const {
  return p_->sk_path.isEmpty();
}

int Path::CountPoints() const {
  return p_->sk_path.countPoints();
}

Point2F Path::GetPoint(int index) const {
  SkPoint p = p_->sk_path.getPoint(index);
  return Point2F(p.fX, p.fY);
}

int Path::GetPoints(Point2F points[], int max) const {
  return p_->sk_path.getPoints(reinterpret_cast<SkPoint *>(points), max);
}

int Path::CountVerbs() const {
  return p_->sk_path.countVerbs();
}

void Path::Swap(Path &other) {
  p_->sk_path.swap(other.p_->sk_path);
}

const RectF &Path::GetBounds() const {
  return reinterpret_cast<const RectF &>(p_->sk_path.getBounds());
}

void Path::UpdateBoundsCache() const {
  p_->sk_path.updateBoundsCache();
}

void Path::MoveTo(float x, float y) {
  p_->sk_path.moveTo(x, y);
}

void Path::MoveTo(const Point2F &p) {
  p_->sk_path.moveTo(reinterpret_cast<const SkPoint &>(p));
}

void Path::RelativeMoveTo(float dx, float dy) {
  p_->sk_path.rMoveTo(dx, dy);
}

void Path::LineTo(float x, float y) {
  p_->sk_path.lineTo(x, y);
}

void Path::LineTo(const Point2F &p) {
  p_->sk_path.lineTo(reinterpret_cast<const SkPoint &>(p));
}

void Path::RelativeLineTo(float dx, float dy) {
  p_->sk_path.rLineTo(dx, dy);
}

void Path::QuadTo(float x1, float y1, float x2, float y2) {
  p_->sk_path.quadTo(x1, y1, x2, y2);
}

void Path::QuadTo(const Point2F &p1, const Point2F &p2) {
  p_->sk_path.quadTo(reinterpret_cast<const SkPoint &>(p1), reinterpret_cast<const SkPoint &>(p2));
}

void Path::RelativeQuadTo(float dx1, float dy1, float dx2, float dy2) {
  p_->sk_path.rQuadTo(dx1, dy1, dx2, dy2);
}

void Path::Close() {
  p_->sk_path.close();
}

bool operator==(const Path &path1, const Path &path2) {
  return (path1.p_->sk_path) == (path2.p_->sk_path);
}

bool operator!=(const Path &path1, const Path &path2) {
  return (path1.p_->sk_path) != (path2.p_->sk_path);
}

} // namespace graphics
} // namespace wiztk
