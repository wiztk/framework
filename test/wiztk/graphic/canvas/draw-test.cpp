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

#include <wiztk/graphic/font-style.hpp>
#include "draw-test.hpp"

#include "wiztk/base/color.hpp"
#include "wiztk/base/rect.hpp"
#include "wiztk/graphic/paint.hpp"
#include "wiztk/graphic/canvas.hpp"
#include "wiztk/graphic/bitmap.hpp"

using namespace wiztk;
using namespace wiztk::base;
using namespace wiztk::graphic;

static const int kWidth = 400;
static const int kHeight = 300;
static const std::string kFileNamePrefix("graphic_canvas_");

DrawTest::DrawTest() {}

DrawTest::~DrawTest() {}

TEST_F(DrawTest, draw_line_1) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(kWidth, kHeight);

  Canvas canvas(bitmap);
  canvas.Clear(0xFFFFFFFF);

  Paint paint;
  paint.SetColor(0xFFFF0000);
  paint.SetStrokeWidth(2.f);
  paint.SetStyle(Paint::kStyleStroke);
  canvas.DrawLine(50.f, 50.f, 350.f, 250.f, paint);
  canvas.Flush();

  std::string filename = kFileNamePrefix + "draw_line_1.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}

TEST_F(DrawTest, draw_rect_1) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(kWidth, kHeight);

  Canvas canvas(bitmap);
  canvas.Clear(0xFFFFFFFF);

  Paint paint;
  paint.SetColor(0xFFFF0000);
  RectF rect = RectF::MakeFromXYWH(0.f, 0.f, kWidth, kHeight);
  rect = rect.Inset(50.f);
  canvas.DrawRect(rect, paint);
  canvas.Flush();

  std::string filename = kFileNamePrefix + "draw_rect_1.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}

TEST_F(DrawTest, draw_text_1) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(kWidth, kHeight);

  Canvas canvas(bitmap);
  canvas.Clear(0xFFFFFFFF);

  Paint paint;
  paint.SetColor(0xFFFF0000);
  paint.SetAntiAlias(true);

  Typeface typeface("Noto Sans CJK SC", FontStyle());
  Font font(typeface, 24.f);
  paint.SetFont(font);

  std::string text = "Hello There!";

  canvas.DrawText(text.c_str(), text.length(), 50.f, 50.f, paint);

  canvas.Flush();

  std::string filename = kFileNamePrefix + "draw_text_1.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}

/**
 * @brief Draw unicode text
 */
TEST_F(DrawTest, draw_text_2) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(kWidth, kHeight);

  Canvas canvas(bitmap);
  canvas.Clear(0xFFFFFFFF);

  Paint paint;
  paint.SetColor(0xFFFF0000);
  paint.SetAntiAlias(true);

  Typeface typeface("Noto Sans CJK SC", FontStyle());
  Font font(typeface, 48.f);
  paint.SetFont(font);

  String16 text(u"你好，骚年！");

//  const char *text = "你好，骚年！";

//  canvas.DrawText(text, strlen(text), 50.f, 50.f, paint);
  canvas.DrawText(text, 50.f, 50.f, paint);

  canvas.Flush();

  std::string filename = kFileNamePrefix + "draw_text_2.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}
