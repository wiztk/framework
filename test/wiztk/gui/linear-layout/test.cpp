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

#include "test.hpp"

#include <wiztk/gui/application.hpp>
#include <wiztk/gui/window.hpp>
#include <wiztk/gui/linear-layout.hpp>

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

/**
 * @brief Show a linear layout in a simple window
 *
 * Expected result: display and resize a default window
 */
TEST_F(Test, regular) {
  using wiztk::gui::Application;
  using wiztk::gui::Window;
  using wiztk::gui::LinearLayout;

  int argc = 1;
  char argv1[] = "show";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  Window win(400, 300, "Test Window");
  win.SetContentView(new LinearLayout);
  win.Show();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}
