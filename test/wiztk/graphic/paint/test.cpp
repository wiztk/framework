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

#include "wiztk/base/color.hpp"
#include "wiztk/graphic/paint.hpp"
#include "wiztk/graphic/canvas.hpp"
#include "wiztk/graphic/bitmap.hpp"
#include "wiztk/graphic/font.hpp"
//#include "wiztk/graphic/font-style.hpp"
#include "wiztk/graphic/gradient-shader.hpp"

#include "SkPaint.h"

using namespace wiztk;
using namespace wiztk::base;
using namespace wiztk::graphic;

static const int kWidth = 400;
static const int kHeight = 300;
static const std::string kFileNamePrefix("graphic_paint_");

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, enums_check_1) {

  // Compare Paint::Style and SkPaint::Style
  ASSERT_TRUE((int) Paint::Style::kStyleFill == (int) SkPaint::Style::kFill_Style);
  ASSERT_TRUE((int) Paint::Style::kStyleStroke == (int) SkPaint::Style::kStroke_Style);
  ASSERT_TRUE((int) Paint::Style::kStyleStrokeAndFill == (int) SkPaint::Style::kStrokeAndFill_Style);

  // Compare Paint::Hinting and SkPaint::Hinting
  ASSERT_TRUE((int) Paint::Hinting::kHintingNone == (int) SkPaint::Hinting::kNo_Hinting);
  ASSERT_TRUE((int) Paint::Hinting::kHintingSlight == (int) SkPaint::Hinting::kSlight_Hinting);
  ASSERT_TRUE((int) Paint::Hinting::kHintingNormal == (int) SkPaint::Hinting::kNormal_Hinting);
  ASSERT_TRUE((int) Paint::Hinting::kHintingFull == (int) SkPaint::Hinting::kFull_Hinting);

}

TEST_F(Test, constructor_1) {
  ColorF blue(0.f, 0.f, 1.f, 1.f);
  ColorF ret;

  Paint paint;
  paint.SetColor(blue);
  ret = paint.GetColor();

  ASSERT_TRUE(ret == blue);
}

TEST_F(Test, text_1) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(kWidth, kHeight);

  Canvas canvas(bitmap);
  canvas.Clear(0xFFFFFFFF);

  Paint paint1, paint2, paint3;
//  Font font("Noto Sans CJK SC", FontStyle(FontStyle::kWeightNormal), 24.f);
  Font font;

  paint1.SetTextSize(64.f);
  paint1.SetAntiAlias(true);
  paint1.SetColor(ColorF::FromUCharRGBA(255, 0, 0));
  paint1.SetFont(font);

  paint2.SetTextSize(64.f);
  paint2.SetAntiAlias(true);
  paint2.SetColor(ColorF::FromUCharRGBA(0, 136, 0));
  paint2.SetStyle(Paint::kStyleStroke);
  paint2.SetStrokeWidth(3.f);
  paint2.SetFont(font);

  paint3.SetTextSize(64.f);
  paint3.SetAntiAlias(true);
  paint3.SetColor(ColorF::FromUCharRGBA(136, 136, 136));
  paint3.SetTextScaleX(1.5f);
  paint3.SetFont(font);

  const char text[] = "Skia!";
  canvas.DrawText(text, strlen(text), 20.f, 64.f, paint1);
  canvas.DrawText(text, strlen(text), 20.f, 144.f, paint2);
  canvas.DrawText(text, strlen(text), 20.f, 224.f, paint3);

  canvas.Flush();

  std::string filename = kFileNamePrefix + "text_1.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}

TEST_F(Test, set_style_1) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(kWidth, kHeight);

  Canvas canvas(bitmap);
  canvas.Clear(0xFFFFFFFF);

  Paint paint1, paint2, paint3;
//  Font font("Noto Sans CJK SC", FontStyle(FontStyle::kWeightNormal), 24.f);
  Font font;

  paint1.SetFont(font);

  paint2.SetStyle(Paint::kStyleStroke);
  paint2.SetStrokeWidth(3.f);

  paint3.SetTextSize(80.f);
  paint3.SetAntiAlias(true);
  paint3.SetColor(ColorF(1.f, 0.f, 0.f));
  paint3.SetFont(font);

  canvas.DrawRect(RectF::MakeFromXYWH(10, 10, 60, 20), paint1);
  canvas.DrawRect(RectF::MakeFromXYWH(80, 10, 60, 20), paint2);

  paint2.SetStrokeWidth(5.f);
  canvas.DrawOval(RectF::MakeFromXYWH(150, 10, 60, 20), paint2);

  canvas.DrawText("SKIA", 4, 20.f, 120.f, paint3);
  paint3.SetColor(ColorF(0.f, 0.f, 1.f));
  canvas.DrawText("SKIA", 4, 20.f, 220.f, paint3);

  canvas.Flush();

  std::string filename = kFileNamePrefix + "set_style_1.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}


TEST_F(Test, set_shader_1) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(256, 256);

  Canvas canvas(bitmap);
  canvas.Clear(0xFFFFFFFF);

  PointF points[2] = {
      PointF(0.f, 0.f),
      PointF(256.f, 256.f)
  };
  uint32_t colors[2] = {0xFF0000FF, 0xFFFFFF00};

  Paint paint;
  paint.SetShader(GradientShader::MakeLinear(points, colors, nullptr, 2, Shader::kTileModeClamp, 0, nullptr));

  canvas.DrawPaint(paint);

  canvas.Flush();

  std::string filename = kFileNamePrefix + "set_shader_1.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}