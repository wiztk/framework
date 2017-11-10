//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <wiztk/graphic/typeface.hpp>
#include <SkTypeface.h>

using namespace wiztk::graphic;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, enums_check_1) {

  ASSERT_TRUE((int) Typeface::kNormal == (int) SkTypeface::kNormal);
  ASSERT_TRUE((int) Typeface::kBold == (int) SkTypeface::kBold);
  ASSERT_TRUE((int) Typeface::kItalic == (int) SkTypeface::kItalic);
  ASSERT_TRUE((int) Typeface::kBoldItalic == (int) SkTypeface::kBoldItalic);

  ASSERT_TRUE((int) Typeface::kEncodingUTF8 == (int) SkTypeface::kUTF8_Encoding);
  ASSERT_TRUE((int) Typeface::kEncodingUTF16 == (int) SkTypeface::kUTF16_Encoding);
  ASSERT_TRUE((int) Typeface::kEncodingUTF32 == (int) SkTypeface::kUTF32_Encoding);

  ASSERT_TRUE(true);
}

TEST_F(Test, default_1) {

  sk_sp<SkTypeface> typeface = SkTypeface::MakeFromName("Noto Sans CJK SC",
                                                        SkFontStyle(SkFontStyle::kNormal_Weight,
                                                                    SkFontStyle::kNormal_Width,
                                                                    SkFontStyle::kUpright_Slant));

  ASSERT_TRUE(typeface);

  SkString name;

  typeface->getFamilyName(&name);
  fprintf(stdout, "%s\n", name.c_str());

  ASSERT_TRUE(true);
}