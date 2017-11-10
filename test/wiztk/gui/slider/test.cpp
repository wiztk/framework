//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <wiztk/gui/application.hpp>
#include <wiztk/gui/window.hpp>
#include <wiztk/gui/slider.hpp>

using namespace wiztk;
using namespace wiztk::gui;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

/*
 *
 */
TEST_F(Test, slider_1) {
  using wiztk::gui::Window;

  int argc = 1;
  char argv1[] = "gui-slider";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  Window *win = new Window(320, 240, "Slider Test");
  win->SetAppId("Test");

  Slider* slider = new Slider;
  win->SetContentView(slider);

  win->Show();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}
