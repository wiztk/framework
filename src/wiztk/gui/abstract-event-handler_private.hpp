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

#ifndef WIZTK_GUI_INTERNAL_ABSTRACT_EVENT_HANDLER_PRIVATE_HPP_
#define WIZTK_GUI_INTERNAL_ABSTRACT_EVENT_HANDLER_PRIVATE_HPP_

#include "wiztk/gui/abstract-event-handler.hpp"

#include "wiztk/base/property.hpp"

namespace wiztk {
namespace gui {

/**
 * @brief The structure for the private data in AbstractEventHandler
 */
struct AbstractEventHandler::Private : public base::Property<AbstractEventHandler> {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);

  explicit Private(AbstractEventHandler *event_handler)
      : base::Property<AbstractEventHandler>(event_handler),
        mouse_event_node(event_handler),
        mouse_motion_event_node(event_handler),
        name() {}

  ~Private() final = default;

  /**
   * @brief An event task to handle mouse enter/leave/button events
   */
  MouseTask mouse_event_node;

  /**
   * @brief An event task to handle mouse move event
   */
  MouseMotionTask mouse_motion_event_node;

  // TODO: there will be more tasks added later

  base::String name;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_INTERNAL_ABSTRACT_EVENT_HANDLER_PRIVATE_HPP_
