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

#include "wiztk/graphics/abstract-surface.hpp"

namespace wiztk {
namespace graphics {

using base::SizeI;
using base::Margin;

void AbstractSurface::SetMargin(const Margin &margin) {
  OnResetMargin(margin.left, margin.top, margin.right, margin.bottom);
}

void AbstractSurface::SetMargin(int left, int top, int right, int bottom) {
  OnResetMargin(left, top, right, bottom);
}

void AbstractSurface::Resize(const Size &size) {
  OnResize(size.width, size.height);
}

void AbstractSurface::Resize(int width, int height) {
  OnResize(width, height);
}

void AbstractSurface::OnResetMargin(int left, int top, int right, int bottom) {
  margin_.left = left;
  margin_.top = top;
  margin_.right = right;
  margin_.bottom = bottom;
}

void AbstractSurface::OnResize(int width, int height) {
  size_.width = width;
  size_.height = height;
}

} // namespace graphics
} // namespace wiztk
