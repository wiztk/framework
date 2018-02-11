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

#include "wiztk/base/property.hpp"

#include "wiztk/gui/main-loop.hpp"
#include "wiztk/gui/view-surface.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

#include <csignal>

namespace wiztk {
namespace gui {

struct MainLoop::Private : public base::Property<MainLoop> {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);
  Private() = delete;

  explicit Private(MainLoop *proprietor)
      : base::Property<MainLoop>(proprietor) {}

  ~Private() final = default;

  bool running = false;

  static const int kMaxEpollEvents = 16;

};

// ----

MainLoop::MainLoop() {
  p_ = std::make_unique<Private>(this);
}

MainLoop::~MainLoop() = default;

void MainLoop::RunLoop() {

}

} // namespace gui
} // namespace wiztk
