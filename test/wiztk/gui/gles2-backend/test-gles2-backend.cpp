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

#include "test-gles2-backend.hpp"

#include "wiztk/gui/application.hpp"
#include "wiztk/gui/window.hpp"
#include "wiztk/gui/push-button.hpp"
#include "wiztk/gui/gles2-backend.hpp"

using namespace wiztk::gui;
using namespace wiztk::graphics;

class Responder : public wiztk::base::Trackable {
 public:
  Responder() = default;
  ~Responder() override = default;

  void OnCheckGLES2Backend(__SLOT__);
};

void Responder::OnCheckGLES2Backend(wiztk::base::SLOT /*slot*/) {
  GLES2Backend backend1;
  ASSERT_TRUE(backend1.IsValid());

  GLES2Backend backend2;
  ASSERT_TRUE(backend2.IsValid());

  fprintf(stdout, "1: major: %d, minor: %d\n", backend1.GetVersionMajor(), backend1.GetVersionMinor());
  fprintf(stdout, "2: major: %d, minor: %d\n", backend2.GetVersionMajor(), backend2.GetVersionMinor());
}

/**
 * @brief Show a default empty window
 *
 * Expected result: display and resize a default window
 */
TEST_F(TestGLES2Backend, show) {
  int argc = 1;
  char argv1[] = "show";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  Responder responder;

  wiztk::gui::Window win(400, 300, "Test GLES2Backend");

  auto *button = PushButton::Create("Click to Run");
  button->clicked().Connect(&responder, &Responder::OnCheckGLES2Backend);

  win.SetContentView(button);
  win.Show();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}
