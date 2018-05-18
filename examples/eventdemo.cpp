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

#include <wiztk/gui/application.hpp>
#include <wiztk/gui/window.hpp>
#include <wiztk/gui/push-button.hpp>
#include <wiztk/gui/relative-layout.hpp>

using namespace wiztk;
using namespace wiztk::gui;

int main(int argc, char *argv[]) {
  Application app(argc, argv);

  Window win(400, 300, "Event Demo");
  win.SetAppId("Event-Demo");

  auto *layout = new RelativeLayout;
  auto *button = PushButton::Create("Test");

  layout->AddView(button);
  button->MoveTo(200, 200);

//  button->AddAnchorTo(layout, wiztk::kAlignLeft, 20);
//  button->AddAnchorTo(layout, wiztk::kAlignTop, 20);
  button->AddAnchorTo(layout, wiztk::kAlignRight, 20);
//  button->AddAnchorTo(layout, wiztk::kAlignBottom, 20);

  win.SetContentView(layout);
  win.Show();

  return app.Run();
}
