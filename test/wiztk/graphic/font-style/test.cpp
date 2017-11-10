//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <wiztk/graphic/font-style.hpp>

#include "SkFontStyle.h"

using namespace wiztk::graphic;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, enums_check_1) {

  // Compare Font::Weight and SkFontStyle::Weight
  ASSERT_TRUE((int) FontStyle::Weight::kWeightInvisible == (int) SkFontStyle::Weight::kInvisible_Weight);
  ASSERT_TRUE((int) FontStyle::Weight::kWeightThin == (int) SkFontStyle::Weight::kThin_Weight);
  ASSERT_TRUE((int) FontStyle::Weight::kWeightExtraLight == (int) SkFontStyle::Weight::kExtraLight_Weight);
  ASSERT_TRUE((int) FontStyle::Weight::kWeightLight == (int) SkFontStyle::Weight::kLight_Weight);
  ASSERT_TRUE((int) FontStyle::Weight::kWeightNormal == (int) SkFontStyle::Weight::kNormal_Weight);
  ASSERT_TRUE((int) FontStyle::Weight::kWeightMedium == (int) SkFontStyle::Weight::kMedium_Weight);
  ASSERT_TRUE((int) FontStyle::Weight::kWeightSemiBold == (int) SkFontStyle::Weight::kSemiBold_Weight);
  ASSERT_TRUE((int) FontStyle::Weight::kWeightBold == (int) SkFontStyle::Weight::kBold_Weight);
  ASSERT_TRUE((int) FontStyle::Weight::kWeightExtraBold == (int) SkFontStyle::Weight::kExtraBold_Weight);
  ASSERT_TRUE((int) FontStyle::Weight::kWeightBlack == (int) SkFontStyle::Weight::kBlack_Weight);
  ASSERT_TRUE((int) FontStyle::Weight::kWeightExtraBlack == (int) SkFontStyle::Weight::kExtraBlack_Weight);

  // Compare FontStyle::Width and SkFontStyle::Width
  ASSERT_TRUE((int) FontStyle::Width::kWidthUltraCondensed == (int) SkFontStyle::kUltraCondensed_Width);
  ASSERT_TRUE((int) FontStyle::Width::kWidthExtraCondensed == (int) SkFontStyle::kExtraCondensed_Width);
  ASSERT_TRUE((int) FontStyle::Width::kWidthCondensed == (int) SkFontStyle::kCondensed_Width);
  ASSERT_TRUE((int) FontStyle::Width::kWidthSemiCondensed == (int) SkFontStyle::kSemiCondensed_Width);
  ASSERT_TRUE((int) FontStyle::Width::kWidthNormal == (int) SkFontStyle::kNormal_Width);
  ASSERT_TRUE((int) FontStyle::Width::kWidthSemiExpanded == (int) SkFontStyle::kSemiExpanded_Width);
  ASSERT_TRUE((int) FontStyle::Width::kWidthExpanded == (int) SkFontStyle::kExpanded_Width);
  ASSERT_TRUE((int) FontStyle::Width::kWidthExtraExpanded == (int) SkFontStyle::kExtraExpanded_Width);
  ASSERT_TRUE((int) FontStyle::Width::kWidthUltraExpanded == (int) SkFontStyle::kUltraExpanded_Width);

  // Compare FontStyle::Slant and SkFontStyle::Slant
  ASSERT_TRUE((int) FontStyle::Slant::kSlantUpright == (int) SkFontStyle::kUpright_Slant);
  ASSERT_TRUE((int) FontStyle::Slant::kSlantItalic == (int) SkFontStyle::kItalic_Slant);
  ASSERT_TRUE((int) FontStyle::Slant::kSlantOblique == (int) SkFontStyle::kOblique_Slant);

}
