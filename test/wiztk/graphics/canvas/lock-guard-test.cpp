//
// Created by zhanggyb on 17-8-25.
//

#include "lock-guard-test.hpp"

#include "wiztk/base/color.hpp"
#include "wiztk/base/rect.hpp"
#include "wiztk/graphics/paint.hpp"
#include "wiztk/graphics/canvas.hpp"
#include "wiztk/graphics/bitmap.hpp"

using namespace wiztk;
using namespace wiztk::base;
using namespace wiztk::graphics;

static const int kWidth = 400;
static const int kHeight = 300;
static const std::string kFileNamePrefix("graphic_canvas_");

LockGuardTest::LockGuardTest() {}

LockGuardTest::~LockGuardTest() {}

TEST_F(LockGuardTest, clip_1) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(kWidth, kHeight);

  Canvas canvas(bitmap);
  canvas.Clear(0xFFFFFFFF);

  Paint paint;
  RectF rect = RectF::FromXYWH(0.f, 0.f, kWidth, kHeight);

  {
    RectF r1 = rect.Inset(50.f);
    Canvas::LockGuard lock(&canvas, r1, ClipOperation::kClipDifference);
    paint.SetColor(ColorF(0.f, 1.f, 0.f)); // green
    canvas.DrawRect(rect, paint);
  }

//  int count = canvas.GetSaveCount();
//  canvas.Save();
//  std::cout << canvas.GetSaveCount() << std::endl;
//
//  canvas.ClipRect(r1, kClipDifference);
//  paint.SetColor(ColorF(0.f, 1.f, 0.f)); // green
//  canvas.DrawRect(rect, paint);
//
//  canvas.RestoreToCount(count);

  paint.SetColor(ColorF(1.f, 0.f, 0.f, 0.5f));  // red
  canvas.DrawRect(rect, paint);

  canvas.Flush();

  std::string filename = kFileNamePrefix + "lockguard_clip_1.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}

TEST_F(LockGuardTest, clip_2) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(kWidth, kHeight);

  Canvas canvas(bitmap);
  canvas.Clear(0xFFFFFFFF); // fill with white

  Paint paint;
  RectF rect = RectF::FromXYWH(0.f, 0.f, kWidth, kHeight);

  Canvas::LockGuard lock1(&canvas);
  Canvas::LockGuard lock2(&canvas);

  RectF r1 = rect.Inset(50.f);
  lock1.Lock(r1, ClipOperation::kClipIntersect);
  paint.SetColor(ColorF(0.f, 1.f, 0.f)); // green
  canvas.DrawRect(rect, paint);

  RectF r2 = r1.Inset(50.f);
  lock2.Lock(r2, ClipOperation::kClipIntersect);
  paint.SetColor(ColorF(0.f, 0.f, 1.f));  // blue
  canvas.DrawRect(r1, paint);

  lock1.Unlock(); // This will make lock2 invalid

  paint.SetColor(ColorF(1.f, 0.f, 0.f, 0.5f));  // red
  canvas.DrawRect(rect, paint);

  canvas.Flush();

  std::string filename = kFileNamePrefix + "lockguard_clip_2.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}
