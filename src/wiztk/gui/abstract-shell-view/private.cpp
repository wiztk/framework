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

#include "private.hpp"

#include "wiztk/gui/context.hpp"
#include "wiztk/gui/mouse-event.hpp"

#include "wiztk/base/bit.hpp"

namespace wiztk {
namespace gui {

using base::Bit;

void AbstractShellView::Private::OnXdgSurfaceConfigure(uint32_t serial) {
  ViewSurface::Shell *shell = ViewSurface::Shell::Get(shell_surface);
  shell->AckConfigure(serial);

  if (!proprietor()->IsShown()) {
    Bit::Set<int>(flags, kFlagMaskShown);
    shell->ResizeWindow(size.width, size.height);
    proprietor()->OnShown();
  }
}

void AbstractShellView::Private::OnXdgToplevelConfigure(int width, int height, int states) {
  bool maximized = (0 != (states & ViewSurface::Shell::Toplevel::kStateMaskMaximized));
  bool fullscreen = (0 != (states & ViewSurface::Shell::Toplevel::kStateMaskFullscreen));
  bool resizing = (0 != (states & ViewSurface::Shell::Toplevel::kStateMaskResizing));
  bool focus = (0 != (states & ViewSurface::Shell::Toplevel::kStateMaskActivated));

  if (maximized != proprietor()->IsMaximized()) {
    Bit::Inverse<int>(flags, kFlagMaskMaximized);
    proprietor()->OnMaximized(maximized);
  }

  if (fullscreen != proprietor()->IsFullscreen()) {
    Bit::Inverse<int>(flags, kFlagMaskFullscreen);
    proprietor()->OnFullscreen(fullscreen);
  }

  if (resizing != proprietor()->IsResizing()) {
    // TODO: no need to use this flag
    Bit::Inverse<int>(flags, kFlagMaskResizing);
  }

  if (focus != proprietor()->IsFocused()) {
    Bit::Inverse<int>(flags, kFlagMaskFocused);
    proprietor()->OnFocus(focus);
  }

  if (width > 0 && height > 0) {
    proprietor()->OnConfigureSize(last_size, Size(width, height));
  }
}

void AbstractShellView::Private::OnXdgToplevelClose() {
  proprietor()->Close();
}

void AbstractShellView::Private::DispatchMouseEnterEvent(AbstractView *parent,
                                                         MouseEvent *event,
                                                         MouseTask *tail) {
  AbstractView *sub = parent->DispatchMouseEnterEvent(event);
  while (nullptr != sub) {
    _ASSERT(sub != parent);
    sub->OnMouseEnter(event);
    if (event->IsAccepted()) {
      tail->push_back(MouseTask::Get(sub));
      tail = static_cast<MouseTask *>(tail->next());
      parent = sub;
      sub = parent->DispatchMouseEnterEvent(event);
    } else if (event->IsIgnored()) {
      parent = sub;
      sub = parent->DispatchMouseEnterEvent(event);
    } else {
      break;
    }
  }
}

}
}