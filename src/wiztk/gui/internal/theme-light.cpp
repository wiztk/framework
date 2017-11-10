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

#include "theme-light.hpp"

#include <wiztk/gui/theme.hpp>

namespace wiztk {
namespace gui {

/**
 * @brief The default light theme
 */
class ThemeLight : public Theme {

 public:

  ThemeLight()
      : Theme() {
    data().window.inactive.outline.colors[0] = 0xFF444444;
    data().window.inactive.background.colors[0] = 0xEFF0F0F0;
    data().window.active.background.colors[0] = 0xEFF0F0F0;
    data().window.inactive.foreground.colors[0] = 0xEFE0E0E0;
    data().window.active.foreground.colors[0] = 0xEFE7E7E7;

    data().title_bar.inactive.background.colors[0] = 0xEFF0F0F0;

    data().title_bar.active.background.colors.resize(2);
    data().title_bar.active.background.colors[0] = 0xFFDDDDDD;
    data().title_bar.active.background.colors[1] = 0xFFCCCCCC;
    data().title_bar.active.background.color_positions.resize(2);
    data().title_bar.active.background.color_positions[0] = 0.f;
    data().title_bar.active.background.color_positions[1] = 1.f;

    data().title_bar.highlight.background.colors[0] = data().title_bar.active.background.colors[0] + 55;
    data().title_bar.inactive.foreground.colors[0] = 0xFF444444;
    data().title_bar.active.foreground.colors[0] = 0xFF777777;
    data().title_bar.highlight.foreground.colors[0] = data().title_bar.active.foreground.colors[0] + 55;

    data().button.inactive.outline.colors[0] = 0xEF999999;
    data().button.inactive.background.colors[0] = 0xFFE0E0E0;
    data().button.inactive.background.colors.resize(2);
    data().button.inactive.background.colors[0] = 0xFFDDDDDD;
    data().button.inactive.background.colors[1] = 0xFFCCCCCC;
    data().button.inactive.background.color_positions.resize(2);
    data().button.inactive.background.color_positions[0] = 0.f;
    data().button.inactive.background.color_positions[1] = 1.f;
    data().button.inactive.foreground.colors[0] = 0xFF444444;
    data().button.active.background.colors[0] = 0xFFB67E3E;
    data().button.active.background.colors.resize(2);
    data().button.active.background.colors[0] = 0xFFAAAAAA;
    data().button.active.background.colors[1] = 0xFFBBBBBB;
    data().button.active.background.color_positions.resize(2);
    data().button.active.background.color_positions[0] = 0.f;
    data().button.active.background.color_positions[1] = 1.f;
    data().button.active.foreground.colors[0] = 0xFFEEEEEE;
    data().button.highlight.background.colors[0] = data().button.inactive.background.colors[0] + 25;
    data().button.highlight.background.colors.resize(2);
    data().button.highlight.background.colors[0] = 0xFFE7E7E7;
    data().button.highlight.background.colors[1] = 0xFFD5D5D5;
    data().button.highlight.background.color_positions.resize(2);
    data().button.highlight.background.color_positions[0] = 0.f;
    data().button.highlight.background.color_positions[1] = 1.f;
    data().button.highlight.foreground.colors[0] = data().button.active.foreground.colors[0] + 25;
  }

  ~ThemeLight() {}

};

}
}

void *ThemeLightCreate() {
  return new wiztk::gui::ThemeLight;
}

void ThemeLightDestroy(void *p) {
  wiztk::gui::ThemeLight *theme = static_cast<wiztk::gui::ThemeLight *>(p);
  delete theme;
}

