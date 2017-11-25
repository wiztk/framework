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

#include "internal/abstract-rendering-api_private.hpp"

namespace wiztk {
namespace gui {

AbstractRenderingAPI::AbstractRenderingAPI() {

}

AbstractRenderingAPI::~AbstractRenderingAPI() {
  destroyed_.Emit();
}

void AbstractRenderingAPI::Setup(ViewSurface *surface) {
  if (surface->p_->rendering_api == this) return;

  if (nullptr != surface->p_->rendering_api) {
    AbstractRenderingAPI *orig = surface->p_->rendering_api;
    surface->p_->rendering_api = nullptr;
    orig->OnRelease(surface);
  }

  surface->p_->rendering_api = this;

  OnSetup(surface);
}

void AbstractRenderingAPI::Release(ViewSurface *surface) {
  if (surface->p_->rendering_api != this) return;

  surface->p_->rendering_api = nullptr;
  OnRelease(surface);
}

} // namespace gui
} // namespace wiztk
