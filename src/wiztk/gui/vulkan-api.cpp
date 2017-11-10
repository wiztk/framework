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

#include "wiztk/gui/vulkan-api.hpp"

#include "internal/display_proxy.hpp"
#include "internal/surface_private.hpp"
#include "internal/abstract-rendering-api_proxy.hpp"

namespace wiztk {
namespace gui {

VulkanAPI::VulkanAPI()
    : AbstractRenderingAPI() {

}

VulkanAPI::~VulkanAPI() {

}

void VulkanAPI::OnSetup() {
  vk::WaylandSurfaceCreateInfoKHR info = {
      vk::WaylandSurfaceCreateFlagsKHR(),
      Display::Proxy::wl_display(),
//      Proxy::GetWaylandSurface(GetSurface())
  };

  vk_surface_ = Display::Proxy::vk_instance().createWaylandSurfaceKHR(info);
}

} // namespace gui
} // namespace wiztk
