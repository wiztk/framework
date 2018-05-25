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
    data().window.inactive.foreground = 0xEFE0E0E0;
    data().window.inactive.background = 0xEFF0F0F0;
    data().window.inactive.outline = 0xFF444444;

    data().window.active.background = 0xEFF0F0F0;
    data().window.active.foreground = 0xEFE7E7E7;

    data().title_bar.active.background = 0xFFDDDDDD;
    data().title_bar.inactive.background = 0xEFF0F0F0;

    data().title_bar.highlight.background = data().title_bar.active.background + 55;
    data().title_bar.inactive.foreground = 0xFF444444;
    data().title_bar.active.foreground = 0xFF777777;
    data().title_bar.highlight.foreground = data().title_bar.active.foreground + 55;

    data().button.inactive.outline = 0xEF999999;
    data().button.inactive.background = 0xFFE0E0E0;
    data().button.inactive.foreground = 0xFF444444;
    data().button.active.background = 0xFFB67E3E;
    data().button.active.foreground = 0xFFEEEEEE;
    data().button.highlight.background = data().button.inactive.background + 25;
    data().button.highlight.foreground = data().button.active.foreground + 25;
  }

  ~ThemeLight() final = default;

};

}
}

void *ThemeLightCreate() {
  return new wiztk::gui::ThemeLight;
}

void ThemeLightDestroy(void *p) {
  auto *theme = static_cast<wiztk::gui::ThemeLight *>(p);
  delete theme;
}

