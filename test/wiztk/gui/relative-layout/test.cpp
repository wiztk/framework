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

#include "wiztk/graphics/alignment.hpp"

#include <wiztk/gui/application.hpp>
#include <wiztk/gui/window.hpp>
#include <wiztk/gui/relative-layout.hpp>
#include <wiztk/gui/push-button.hpp>
#include <wiztk/gui/anchor-group.hpp>
#include <wiztk/gui/anchor.hpp>

using namespace wiztk;
using namespace wiztk::gui;
using namespace wiztk::graphics;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

/**
 * @brief Show a relative layout in a simple window
 *
 * Expected result: display and resize a default window
 */
TEST_F(Test, regular_1) {
  int argc = 1;
  char argv1[] = "show";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  Window win(400, 300, "Test Window");

  RelativeLayout *layout = new RelativeLayout;
  PushButton *button = PushButton::Create("Test");

  layout->AddView(button);
  button->MoveTo(100, 100);

  win.SetContentView(layout);
  win.Show();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}

/**
 * @brief Show a relative layout in a simple window
 *
 * Expected result: display and resize a default window
 */
TEST_F(Test, layout_left) {
  int argc = 1;
  char argv1[] = "show";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  Window win(400, 300, "Relative Layout - Left");

  RelativeLayout *layout = new RelativeLayout;
  PushButton *button = PushButton::Create("Test");

  layout->AddView(button);
  button->MoveTo(200, 200);

  button->AddAnchorTo(layout, Alignment::kLeft, 20);
//  button->AddAnchorTo(layout, wiztk::kAlignTop, 20);
//  button->AddAnchorTo(layout, wiztk::kAlignRight, 20);
//  button->AddAnchorTo(layout, wiztk::kAlignBottom, 20);

  win.SetContentView(layout);
  win.Show();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}

/**
 * @brief Show a relative layout in a simple window
 *
 * Expected result: display and resize a default window
 */
TEST_F(Test, layout_top) {
  int argc = 1;
  char argv1[] = "show";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  Window win(400, 300, "Relative Layout - Top");

  RelativeLayout *layout = new RelativeLayout;
  PushButton *button = PushButton::Create("Test");

  layout->AddView(button);
  button->MoveTo(200, 200);

//  button->AddAnchorTo(layout, wiztk::kAlignLeft, 20);
  button->AddAnchorTo(layout, Alignment::kTop, 20);
//  button->AddAnchorTo(layout, wiztk::kAlignRight, 20);
//  button->AddAnchorTo(layout, wiztk::kAlignBottom, 20);

  win.SetContentView(layout);
  win.Show();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}

/**
 * @brief Show a relative layout in a simple window
 *
 * Expected result: display and resize a default window
 */
TEST_F(Test, layout_right) {
  int argc = 1;
  char argv1[] = "show";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  Window win(400, 300, "Relative Layout - Right");

  RelativeLayout *layout = new RelativeLayout;
  PushButton *button = PushButton::Create("Test");

  layout->AddView(button);
  button->MoveTo(200, 200);

//  button->AddAnchorTo(layout, wiztk::kAlignLeft, 20);
//  button->AddAnchorTo(layout, wiztk::kAlignTop, 20);
  button->AddAnchorTo(layout, Alignment::kRight, 20);
//  button->AddAnchorTo(layout, wiztk::kAlignBottom, 20);

  win.SetContentView(layout);
  win.Show();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}

/**
 * @brief Show a relative layout in a simple window
 *
 * Expected result: display and resize a default window
 */
TEST_F(Test, layout_bottom) {
  int argc = 1;
  char argv1[] = "show";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  Window win(400, 300, "Relative Layout - Bottom");

  RelativeLayout *layout = new RelativeLayout;
  PushButton *button = PushButton::Create("Test");

  layout->AddView(button);
  button->MoveTo(200, 200);

//  button->AddAnchorTo(layout, wiztk::kAlignLeft, 20);
//  button->AddAnchorTo(layout, wiztk::kAlignTop, 20);
//  button->AddAnchorTo(layout, wiztk::kAlignRight, 20);
  button->AddAnchorTo(layout, Alignment::kBottom, 20);

  win.SetContentView(layout);
  win.Show();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}

/**
 * @brief Show a relative layout in a simple window
 *
 * Expected result: display and resize a default window
 */
TEST_F(Test, regular_2) {
  int argc = 1;
  char argv1[] = "show";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  Window win(400, 300, "Test Window");

  RelativeLayout *layout = new RelativeLayout;
  PushButton *button = PushButton::Create("Test");

  layout->AddView(button);
  button->MoveTo(200, 200);

  button->AddAnchorTo(layout, Alignment::kLeft, 20);
  button->AddAnchorTo(layout, Alignment::kTop, 20);
  button->AddAnchorTo(layout, Alignment::kRight, 20);
  button->AddAnchorTo(layout, Alignment::kBottom, 20);

  win.SetContentView(layout);
  win.Show();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}
