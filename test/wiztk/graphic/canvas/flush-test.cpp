//
// Created by zhanggyb on 17-9-8.
//

#include "flush-test.hpp"

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

TEST_F(FlushTest, flush_1) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(kWidth, kHeight);

  Canvas canvas(bitmap);

  canvas.Clear(0xFFFFFFFF);

  Paint paint;
  paint.SetColor(0xFFFF0000);
  RectF rect = RectF::FromXYWH(0.f, 0.f, kWidth, kHeight);
  rect = rect.Inset(50.f);
  canvas.DrawRect(rect, paint);

  canvas.Save();
  canvas.ClipRect(RectF::FromXYWH(40, 40, 200, 200));
  canvas.Clear(0x0);

  paint.SetColor(0xFF0000FF);
  canvas.DrawRect(RectF::FromXYWH(100, 100, 80, 100), paint);

  canvas.Restore();

  canvas.Flush();

  std::string filename = kFileNamePrefix + "flush_1.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}