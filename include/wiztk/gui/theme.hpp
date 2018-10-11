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

#ifndef WIZTK_GUI_THEME_HPP_
#define WIZTK_GUI_THEME_HPP_

#include "wiztk/base/color.hpp"
#include "wiztk/base/thickness.hpp"
#include "wiztk/base/point.hpp"

#include "wiztk/graphics/shader.hpp"
#include "wiztk/graphics/font.hpp"
#include "wiztk/graphics/pixmap.hpp"

#include <vector>
#include <string>

class SkPixmap;

namespace wiztk {
namespace gui {

typedef void *(*ThemeCreateHandle)();
typedef void(*ThemeDestroyHandle)(void *p);

/**
 * @ingroup gui
 * @brief The global theme manager
 */
class WIZTK_EXPORT Theme {

  friend class Application;

 public:

  using ColorF = base::ColorF;
  using Margin = base::ThicknessI;

  Theme(const Theme &) = delete;
  Theme &operator=(const Theme &) = delete;

  struct Schema {

    struct Style {
      ColorF foreground;
      ColorF background;
      ColorF outline;
    };

    Schema() {

      active.foreground = 0xFF000000; // black
      active.background = 0xFFFFFFFF; // white
      active.outline = 0xFF000000;  // black

      inactive = active;
      highlight = active;

    }

    ~Schema() = default;

    // TODO: use image

    Style active;
    Style inactive;
    Style highlight;

  };

  struct Data {

    Data();

    std::string name;

    Schema window;

    Schema title_bar;

    graphics::Font title_bar_font;

    Schema button;

    graphics::Font default_font;

  };

  static void Load(const char *name = nullptr);

  static inline int GetShadowRadius() {
    return kShadowRadius;
  }

  static inline int GetShadowOffsetX() {
    return kShadowOffsetX;
  }

  static inline int GetShadowOffsetY() {
    return kShadowOffsetY;
  }

  static inline const Margin &GetShadowMargin() {
    return kShadowMargin;
  }

  static inline const graphics::Pixmap *GetShadowPixmap() {
    return kShadowPixmap;
  }

  static const int kShadowImageWidth = 250;

  static const int kShadowImageHeight = 250;

  static const Data &GetData() { return kTheme->data_; }

 protected:

  Theme();

  virtual ~Theme();

  Data &data() { return data_; }

 private:

  /**
   * @brief Initialize static properties
   *
   * This method is called only in Application
   */
  static void Initialize();

  /**
   * @brief Release the memory allocated for theme
   *
   * This method is called only in Application
   */
  static void Release();

  static void GenerateShadowImage();

  static int kShadowRadius;

  static int kShadowOffsetX;
  static int kShadowOffsetY;

  static Margin kShadowMargin;

  static std::vector<uint32_t> kShadowPixels;

  static graphics::Pixmap *kShadowPixmap;

  static Theme *kTheme;

  Data data_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_THEME_HPP_
