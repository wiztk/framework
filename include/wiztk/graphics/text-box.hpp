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

#ifndef WIZTK_GRAPHIC_TEXT_BOX_HPP_
#define WIZTK_GRAPHIC_TEXT_BOX_HPP_

#include "../base/rect.hpp"

#include <memory>

namespace wiztk {
namespace graphics {

class Canvas;
class Paint;

class TextBox {

 public:

  enum Mode {
    kModeOneLine,
    kModeLineBreak,

    kModeCount
  };

  enum SpacingAlign {
    kSpacingAlignStart,
    kSpacingAlignCenter,
    kSpacingAlignEnd,

    kSpacingAlignCount
  };

  TextBox();

  ~TextBox();

  Mode GetMode() const;

  void SetMode(Mode mode);

  SpacingAlign GetSpacingAlign() const;

  void SetSpacingAlign(SpacingAlign align);

  void GetBox(base::RectF *rect) const;

  void SetBox(const base::RectF &rect);

  void SetBox(float left, float top, float right, float bottom);

  void GetSpacing(float *mul, float *add) const;

  void SetSpacing(float mul, float add);

  void Draw(const Canvas &canvas, const char text[], size_t len, const Paint &paint);

  void SetText(const char text[], size_t len, const Paint &paint);

  void Draw(const Canvas &canvas);

  int CountLines() const;

  float GetTextHeight() const;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHIC_TEXT_BOX_HPP_
