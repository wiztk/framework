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

#include "matrix-test.hpp"

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

MatrixTest::MatrixTest()
    : testing::Test() {
}

MatrixTest::~MatrixTest() {

}

TEST_F(MatrixTest, draw_rect_1) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(kWidth, kHeight);

  Canvas canvas(bitmap);

  Paint paint;
  paint.SetColor(0x7FFF0000);
  RectF rect = RectF::FromXYWH(0.f, 0.f, kWidth, kHeight);
  rect = rect.Inset(50.f);
  canvas.DrawRect(rect, paint);
  canvas.Flush();

  std::string filename = kFileNamePrefix + "matrix_translate_1.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}

TEST_F(MatrixTest, lock_guard_1) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(kWidth, kHeight);

  Canvas canvas(bitmap);

  Paint paint;
  paint.SetColor(ColorF(1.f, 0.f, 0.f));  // red
  RectF rect = RectF::FromXYWH(0.f, 0.f, kWidth, kHeight);

  canvas.DrawRect(rect, paint);

  {
    RectF r1 = rect.Inset(50.f);
    Canvas::LockGuard lock(&canvas, r1, ClipOperation::kClipIntersect);
    paint.SetColor(ColorF(0.f, 1.f, 0.f)); // green
    canvas.DrawRect(rect, paint);
  }

  canvas.Flush();

  std::string filename = kFileNamePrefix + "matrix_scale_1.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}
