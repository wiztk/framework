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

#include "test-output.hpp"

#include "wiztk/gui/application.hpp"
#include "wiztk/gui/window.hpp"
#include "wiztk/gui/label.hpp"

#include "wiztk/gui/display.hpp"
#include "wiztk/gui/output-manager.hpp"

#include "wiztk/gui/push-button.hpp"

#include <wiztk/graphics/font.hpp>
#include "wiztk/graphics/font-style.hpp"

using namespace wiztk::gui;
using namespace wiztk::graphics;

class Responder : public wiztk::base::Trackable {
 public:
  Responder() = default;
  ~Responder() override = default;

  void OnShowDisplayInfo(__SLOT__);
};

void Responder::OnShowDisplayInfo(wiztk::base::SLOT slot) {
  OutputManager *output_manager = Application::GetInstance()->GetDisplay()->GetOutputManager();
  auto *output = output_manager->GetActiveOutput();

  std::string text("Output info: ");
  text += "Make: ";
  text += output->GetMake();
  text += " Model: ";
  text += output->GetModel();

  std::cout << text << std::endl;
}

/**
 * @brief Show a default empty window
 *
 * Expected result: display and resize a default window
 */
TEST_F(TestOutput, show) {
  int argc = 1;
  char argv1[] = "show";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  Responder responder;

  Window win(400, 300, "Test Output");

  auto *button = PushButton::Create("Get Output");
  button->clicked().Connect(&responder, &Responder::OnShowDisplayInfo);

  win.SetContentView(button);
  win.Show();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}
