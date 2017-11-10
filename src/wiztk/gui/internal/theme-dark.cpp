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

#include "theme-dark.hpp"

#include "wiztk/gui/theme.hpp"

namespace wiztk {
namespace gui {

/**
 * @brief The built-in dark theme
 */
class ThemeDark : public Theme {

 public:

  ThemeDark()
      : Theme() {
    data().window.inactive.outline.colors[0] = 0xEF000000;
    data().window.inactive.background.colors[0] = 0xEF202020;
    data().window.active.background.colors[0] = 0xEF202020;
    data().window.inactive.foreground.colors[0] = 0xEF303030;
    data().window.active.foreground.colors[0] = 0xEF404040;

    data().title_bar.inactive.background.colors[0] = 0xEF202020;

    data().title_bar.active.background.colors.resize(2);
    data().title_bar.active.background.colors[0] = 0xFF303030;
    data().title_bar.active.background.colors[1] = 0xFF292929;
    data().title_bar.active.background.color_positions.resize(2);
    data().title_bar.active.background.color_positions[0] = 0.f;
    data().title_bar.active.background.color_positions[1] = 1.f;

    data().title_bar.highlight.background.colors[0] = data().title_bar.active.background.colors[0] + 55;
    data().title_bar.inactive.foreground.colors[0] = 0xFFAAAAAA;
    data().title_bar.active.foreground.colors[0] = 0xFF999999;
    data().title_bar.highlight.foreground.colors[0] = data().title_bar.active.foreground.colors[0] + 55;

    data().button.inactive.outline.colors[0] = 0x7F000000;
    data().button.inactive.background.colors[0] = 0xFF4E4E4E;
    data().button.inactive.background.colors.resize(2);
    data().button.inactive.background.colors[0] = 0xFF5E5E5E;
    data().button.inactive.background.colors[1] = 0xFF3E3E3E;
    data().button.inactive.background.color_positions.resize(2);
    data().button.inactive.background.color_positions[0] = 0.f;
    data().button.inactive.background.color_positions[1] = 1.f;
    data().button.active.background.colors[0] = 0xFFB67E3E;
    data().button.active.background.colors.resize(2);
    data().button.active.background.colors[0] = 0xFF8B5C2D;
    data().button.active.background.colors[1] = 0xFFCD9866;
    data().button.active.background.color_positions.resize(2);
    data().button.active.background.color_positions[0] = 0.f;
    data().button.active.background.color_positions[1] = 1.f;
    data().button.highlight.background.colors[0] = data().button.inactive.background.colors[0] + 25;
    data().button.inactive.foreground.colors[0] = 0xFF444444;
    data().button.active.foreground.colors[0] = 0xFFEEEEEE;
    data().button.highlight.foreground.colors[0] = data().button.active.foreground.colors[0] + 25;
  }

  ~ThemeDark() {}

};

}
}

void *ThemeDarkCreate() {
  return new wiztk::gui::ThemeDark;
}

void ThemeDarkDestroy(void *p) {
  wiztk::gui::ThemeDark *theme = static_cast<wiztk::gui::ThemeDark *>(p);
  delete theme;
}
