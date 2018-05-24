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

#include <wiztk/gui/application.hpp>
#include <wiztk/gui/window.hpp>
#include <wiztk/gui/label.hpp>

#include <wiztk/graphics/font.hpp>
#include "wiztk/graphics/font-style.hpp"

int main(int argc, char *argv[]) {
  using namespace wiztk;
  using namespace wiztk::gui;
  using namespace wiztk::graphics;

  Application app(argc, argv);

  Window win(320, 240, "Hello");
  win.SetAppId("Hello");

  auto *label = Label::Create("Hello Wayland!");
  label->SetName("Label");
  label->SetForeColor(0xFF444444);
  label->SetFont(Font(24.f));
  win.SetContentView(label);

  win.Show();

  return app.Run();
}
