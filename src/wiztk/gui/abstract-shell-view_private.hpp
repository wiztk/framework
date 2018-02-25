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

#ifndef WIZTK_GUI_INTERNAL_ABSTRACT_SHELL_VIEW_PRIVATE_HPP_
#define WIZTK_GUI_INTERNAL_ABSTRACT_SHELL_VIEW_PRIVATE_HPP_

#include "wiztk/gui/abstract-shell-view.hpp"

#include "wiztk/base/property.hpp"
#include "xdg-shell-unstable-v6-client-protocol.h"

namespace wiztk {
namespace gui {

/**
 * @brief A structure for private data in AbstractShellView
 */
struct AbstractShellView::Private : public base::Property<AbstractShellView> {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);
  Private() = delete;

  enum FlagMask {
    kFlagMaskMaximized = 0x1 << 0,
    kFlagMaskFullscreen = 0x1 << 1,
    kFlagMaskResizing = 0x1 << 2,
    kFlagMaskFocused = 0x1 << 3,
    kFlagMaskMinimized = 0x1 << 4,
    kFlagMaskShown = 0x1 << 5
  };

  /**
   * @brief Constructor
   */
  explicit Private(AbstractShellView *shell_view)
      : base::Property<AbstractShellView>(shell_view),
        flags(0),
        shell_surface(nullptr),
        parent(nullptr),
        geometry_task(shell_view),
        is_damaged(false) {}

  /**
   * @brief Destructor
   */
  ~Private() final = default;

  /**
   * @brief Bitwise flags
   */
  int flags;

  /**
   * @brief Title string for the top level window
   *
   * TODO: support multi-language
   */
  std::string title;

  /**
   * @brief App ID string for the top level window
   */
  std::string app_id;

  /**
   * @brief Current window size
   */
  Size size;

  /**
   * @brief Last valid window size
   */
  Size last_size;

  /**
   * @brief A pointer to a shell surface created with a shell view object
   */
  ViewSurface *shell_surface;

  /**
   * @brief The parent shell view object
   *
   * According to tge xdg-shell protocol, this member variable should be:
   *
   *  - A shell view object if surface is a popup shell surface
   *  - nullptr if surface is a top level shell surface
   */
  AbstractShellView *parent;

  GeometryMessage geometry_task;

  /**
   * @brief If need to call wayland API to damage area on the surface
   */
  bool is_damaged;

  void OnXdgSurfaceConfigure(uint32_t serial);

  void OnXdgToplevelConfigure(int width, int height, int states);

  void OnXdgToplevelClose();

  void DispatchMouseEnterEvent(AbstractView *parent,
                               MouseEvent *event,
                               MouseEventNode *tail);

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_INTERNAL_ABSTRACT_SHELL_VIEW_PRIVATE_HPP_
