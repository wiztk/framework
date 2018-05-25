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
    data().window.inactive.outline = 0xEF0F0F0F;
    data().window.inactive.background = 0xEF202020;
    data().window.inactive.foreground = 0xEF303030;

    data().window.active.background = 0xEF202020;
    data().window.active.foreground = 0xEF404040;

    data().title_bar.inactive.background = 0xEF202020;
    data().title_bar.inactive.foreground = 0xFFAAAAAA;

    data().title_bar.active.background = 0xFF292929;
    data().title_bar.active.foreground = 0xFF777777;

    data().title_bar.highlight.background = data().title_bar.active.background + 55;
    data().title_bar.highlight.foreground = data().title_bar.active.foreground + 55;

    data().button.inactive.outline = 0x7F000000;
    data().button.inactive.background = 0xFF4E4E4E;
    data().button.active.background = 0xFFB67E3E;
    data().button.highlight.background = data().button.inactive.background + 25;
    data().button.inactive.foreground = 0xFF444444;
    data().button.active.foreground = 0xFFEEEEEE;
    data().button.highlight.foreground = data().button.active.foreground + 25;
  }

  ~ThemeDark() final = default;

};

}
}

void *ThemeDarkCreate() {
  return new wiztk::gui::ThemeDark;
}

void ThemeDarkDestroy(void *p) {
  auto *theme = static_cast<wiztk::gui::ThemeDark *>(p);
  delete theme;
}
