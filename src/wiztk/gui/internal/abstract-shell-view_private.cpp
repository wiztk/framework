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

#include "abstract-shell-view_private.hpp"

#include "wiztk/gui/context.hpp"
#include "wiztk/gui/mouse-event.hpp"

#include "wiztk/numerical/bit.hpp"

namespace wiztk {
namespace gui {

using numerical::Bit;

void AbstractShellView::Private::OnXdgSurfaceConfigure(uint32_t serial) {
  ViewSurface::Shell *shell = ViewSurface::Shell::Get(shell_surface);
  shell->AckConfigure(serial);

  if (!owner()->IsShown()) {
    Bit::Set<int>(flags, kFlagMaskShown);
    shell->ResizeWindow(size.width, size.height);
    owner()->OnShown();
  }
}

void AbstractShellView::Private::OnXdgToplevelConfigure(int width, int height, int states) {
  bool maximized = (0 != (states & ViewSurface::Shell::Toplevel::kStateMaskMaximized));
  bool fullscreen = (0 != (states & ViewSurface::Shell::Toplevel::kStateMaskFullscreen));
  bool resizing = (0 != (states & ViewSurface::Shell::Toplevel::kStateMaskResizing));
  bool focus = (0 != (states & ViewSurface::Shell::Toplevel::kStateMaskActivated));

  if (maximized != owner()->IsMaximized()) {
    Bit::Inverse<int>(flags, kFlagMaskMaximized);
    owner()->OnMaximized(maximized);
  }

  if (fullscreen != owner()->IsFullscreen()) {
    Bit::Inverse<int>(flags, kFlagMaskFullscreen);
    owner()->OnFullscreen(fullscreen);
  }

  if (resizing != owner()->IsResizing()) {
    // TODO: no need to use this flag
    Bit::Inverse<int>(flags, kFlagMaskResizing);
  }

  if (focus != owner()->IsFocused()) {
    Bit::Inverse<int>(flags, kFlagMaskFocused);
    owner()->OnFocus(focus);
  }

  if (width > 0 && height > 0) {
    owner()->OnConfigureSize(last_size, Size(width, height));
  }
}

void AbstractShellView::Private::OnXdgToplevelClose() {
  owner()->Close();
}

void AbstractShellView::Private::DispatchMouseEnterEvent(AbstractView *parent, MouseEvent *event, EventTask *tail) {
  AbstractView *sub = parent->DispatchMouseEnterEvent(event);
  while (nullptr != sub) {
    _ASSERT(sub != parent);
    sub->OnMouseEnter(event);
    if (event->IsAccepted()) {
      tail->PushBack(EventTask::GetMouseTask(sub));
      tail = static_cast<EventTask *>(tail->GetNext());
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