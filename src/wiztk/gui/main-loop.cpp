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

  static void HandleSignalInterrupt(int);

  static const int kMaxEpollEvents = 16;

};

void MainLoop::Private::HandleSignalInterrupt(int) {

}

// ----

MainLoop::MainLoop() {
  p_ = std::make_unique<Private>(this);
}

MainLoop::~MainLoop() {

}

void MainLoop::Run() {
  struct sigaction sigint = {};
  sigint.sa_handler = Private::HandleSignalInterrupt;
  sigemptyset(&sigint.sa_mask);
  sigint.sa_flags = SA_RESETHAND;
  sigaction(SIGINT, &sigint, nullptr);

  // TODO: process events.
}

} // namespace gui
} // namespace wiztk
