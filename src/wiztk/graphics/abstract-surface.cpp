/*
 * Copyright 2017 The WizTK Authors.
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

#include "abstract-surface_private.hpp"

namespace wiztk {
namespace graphics {

using base::SizeI;

AbstractSurface::AbstractSurface() {
  p_ = std::make_unique<Private>(this);
}

AbstractSurface::AbstractSurface(const Size &size, const Margin &margin)
    : AbstractSurface() {
  p_->size = size;
  p_->margin = margin;
}

AbstractSurface::AbstractSurface(int width, int height, const Margin &margin)
    : AbstractSurface() {
  p_->size.width = width;
  p_->size.height = height;
  p_->margin = margin;
}

AbstractSurface::AbstractSurface(const Margin &margin)
    : AbstractSurface() {
  p_->margin = margin;
}

AbstractSurface::AbstractSurface(AbstractSurface &&other) noexcept {
  p_ = std::move(other.p_);
}

AbstractSurface &AbstractSurface::operator=(AbstractSurface &&other) noexcept {
  p_ = std::move(other.p_);
  return *this;
}

AbstractSurface::~AbstractSurface() {
  delete p_->backend;
}

void AbstractSurface::SetMargin(const Margin &margin) {
  OnResetMargin(margin.left, margin.top, margin.right, margin.bottom);
}

void AbstractSurface::SetMargin(int left, int top, int right, int bottom) {
  OnResetMargin(left, top, right, bottom);
}

const AbstractSurface::Margin &AbstractSurface::GetMargin() const {
  return p_->margin;
}

void AbstractSurface::Resize(const Size &size) {
  OnResize(size.width, size.height);
}

void AbstractSurface::Resize(int width, int height) {
  OnResize(width, height);
}

const AbstractSurface::Size &AbstractSurface::GetSize() const {
  return p_->size;
}

void AbstractSurface::SetBackend(AbstractBackend *backend) {
  p_->backend = backend;
  // TODO: more control
}

AbstractBackend *AbstractSurface::GetBackend() const {
  return p_->backend;
}

void AbstractSurface::OnResetMargin(int left, int top, int right, int bottom) {
  p_->margin.left = left;
  p_->margin.top = top;
  p_->margin.right = right;
  p_->margin.bottom = bottom;
}

void AbstractSurface::OnResize(int width, int height) {
  p_->size.width = width;
  p_->size.height = height;
}

} // namespace graphics
} // namespace wiztk
