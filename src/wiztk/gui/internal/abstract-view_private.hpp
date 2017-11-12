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

#ifndef WIZTK_GUI_INTERNAL_ABSTRACT_VIEW_PRIVATE_HPP_
#define WIZTK_GUI_INTERNAL_ABSTRACT_VIEW_PRIVATE_HPP_

#include "wiztk/gui/abstract-view.hpp"

#include "wiztk/base/padding.hpp"
#include "wiztk/gui/anchor.hpp"
#include "wiztk/gui/anchor-group.hpp"

namespace wiztk {
namespace gui {

/**
 * @ingroup gui_intern
 * @brief A structure for private data in AbstractView
 */
WIZTK_NO_EXPORT struct AbstractView::Private {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);
  Private() = delete;

  explicit Private(AbstractView *view)
      : previous(nullptr),
        next(nullptr),
        first_child(nullptr),
        last_child(nullptr),
        parent(nullptr),
        children_count(0),
        shell_view(nullptr),
        visible(true),
        minimal_size(0, 0),
        preferred_size(100, 100),
        maximal_size(65536, 65536),
        x_layout_policy(kLayoutPreferred),
        y_layout_policy(kLayoutPreferred),
        geometry_task(view),
        redraw_node(view),
        left_anchor_group(view, kAlignLeft),
        top_anchor_group(view, kAlignTop),
        right_anchor_group(view, kAlignRight),
        bottom_anchor_group(view, kAlignBottom),
        layout(nullptr) {}

  ~Private() = default;

  AbstractView *previous;
  AbstractView *next;

  AbstractView *first_child;
  AbstractView *last_child;

  AbstractView *parent;
  int children_count;

  AbstractShellView *shell_view;

  bool visible;

  Size minimal_size;

  Size preferred_size;

  Size maximal_size;

  LayoutPolicy x_layout_policy;

  LayoutPolicy y_layout_policy;

  Padding padding;

  RectF geometry;

  RectF last_geometry;

  /**
   * @brief The internal origin and size
   */
  RectF bounds;

  GeometryTask geometry_task;

  RenderNode redraw_node;

  AnchorGroup left_anchor_group;
  AnchorGroup top_anchor_group;
  AnchorGroup right_anchor_group;
  AnchorGroup bottom_anchor_group;

  AbstractLayout *layout;

  std::string name;
  
};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_INTERNAL_ABSTRACT_VIEW_PRIVATE_HPP_
