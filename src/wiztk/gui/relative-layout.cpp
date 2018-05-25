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

#include "abstract-view/iterators.hpp"

#include <wiztk/gui/relative-layout.hpp>

namespace wiztk {
namespace gui {

RelativeLayout::~RelativeLayout() {

}

void RelativeLayout::OnViewAdded(AbstractView *view) {
  if (view->IsVisible())
    Layout();
}

void RelativeLayout::OnViewRemoved(AbstractView *view) {
  if (view->IsVisible())
    Layout();
}

void RelativeLayout::OnLayout(int left, int top, int right, int bottom) {
  const AnchorGroup &left_group = GetAnchorGroup(kAlignLeft);
  for (Anchor *anchor = left_group.first(); nullptr != anchor; anchor = anchor->next()) {
    anchor->contrary()->group()->view()->SetLeft(left + anchor->distance());
  }

  const AnchorGroup &top_group = GetAnchorGroup(kAlignTop);
  for (Anchor *anchor = top_group.first(); nullptr != anchor; anchor = anchor->next()) {
    anchor->contrary()->group()->view()->SetTop(top + anchor->distance());
  }

  const AnchorGroup &right_group = GetAnchorGroup(kAlignRight);
  for (Anchor *anchor = right_group.first(); nullptr != anchor; anchor = anchor->next()) {
    anchor->contrary()->group()->view()->SetRight(right + anchor->distance());
  }

  const AnchorGroup &bottom_group = GetAnchorGroup(kAlignBottom);
  for (Anchor *anchor = bottom_group.first(); nullptr != anchor; anchor = anchor->next()) {
    anchor->contrary()->group()->view()->SetBottom(bottom + anchor->distance());
  }
}

} // namespace gui
} // namespace wiztk
