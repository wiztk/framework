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

#ifndef WIZTK_GUI_MAIN_LOOP_HPP_
#define WIZTK_GUI_MAIN_LOOP_HPP_

#include "wiztk/system/async/event-loop.hpp"

#include <memory>

namespace wiztk {
namespace gui {

/**
 * @ingroup gui
 * @brief The main event loop used in Application.
 */
class MainLoop : public system::async::EventLoop {

  friend class Application;

 public:

  MainLoop();

  ~MainLoop() final;

  void RunLoop();

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_MAIN_LOOP_HPP_
