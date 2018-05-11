//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <wiztk/graphics/font.hpp>
#include "SkFont.h"

using namespace wiztk::graphics;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, enums_check_1) {
  ASSERT_TRUE((int) TextEncoding::kTextEncodingUTF8 == (int) SkTextEncoding::kUTF8_SkTextEncoding);
  ASSERT_TRUE((int) TextEncoding::kTextEncodingUTF16 == (int) SkTextEncoding::kUTF16_SkTextEncoding);
  ASSERT_TRUE((int) TextEncoding::kTextEncodingUTF32 == (int) SkTextEncoding::kUTF32_SkTextEncoding);
  ASSERT_TRUE((int) TextEncoding::kTextEncodingGlyphID == (int) SkTextEncoding::kGlyphID_SkTextEncoding);

  ASSERT_TRUE((int) Font::Flags::kFlagEnableAutoHints == (int) SkFont::Flags::kEnableAutoHints_Flag);
  ASSERT_TRUE((int) Font::Flags::kFlagEnableByteCodeHints == (int) SkFont::Flags::kEnableByteCodeHints_Flag);
  ASSERT_TRUE((int) Font::Flags::kFlagEmbeddedBitmaps == (int) SkFont::Flags::kEmbeddedBitmaps_Flag);
  ASSERT_TRUE((int) Font::Flags::kFlagUseNonlinearMetrics == (int) SkFont::Flags::kUseNonlinearMetrics_Flag);
  ASSERT_TRUE((int) Font::Flags::kFlagVertical == (int) SkFont::Flags::kVertical_Flag);
  ASSERT_TRUE((int) Font::Flags::kFlagGenA8FromLCD == (int) SkFont::Flags::kGenA8FromLCD_Flag);
  ASSERT_TRUE((int) Font::Flags::kFlagEmbolden == (int) SkFont::Flags::kEmbolden_Flag);
  ASSERT_TRUE((int) Font::Flags::kFlagDevKern == (int) SkFont::Flags::kDevKern_Flag);

  ASSERT_TRUE((int) Font::MaskType::kMaskBW == (int) SkFont::MaskType::kBW_MaskType);
  ASSERT_TRUE((int) Font::MaskType::kMaskA8 == (int) SkFont::MaskType::kA8_MaskType);
  ASSERT_TRUE((int) Font::MaskType::kMaskLCD == (int) SkFont::MaskType::kLCD_MaskType);
}
