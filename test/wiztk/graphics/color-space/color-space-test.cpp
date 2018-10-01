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

#include "color-space-test.hpp"

#include <wiztk/graphics/color-space.hpp>

#include "SkColorSpace.h"

using namespace wiztk::graphics;

TEST_F(ColorSpaceTest, color_space_gamut_enum) {

  // Compare Font::Weight and SkFontStyle::Weight
  ASSERT_TRUE((int) ColorSpace::GamutType::kGamutSRGB == (int) SkColorSpace::Gamut::kSRGB_Gamut);
  ASSERT_TRUE((int) ColorSpace::GamutType::kGamutAdobeRGB == (int) SkColorSpace::Gamut::kAdobeRGB_Gamut);
  ASSERT_TRUE((int) ColorSpace::GamutType::kGamutDCIP3_D65 == (int) SkColorSpace::Gamut::kDCIP3_D65_Gamut);
  ASSERT_TRUE((int) ColorSpace::GamutType::kGamutRec2020 == (int) SkColorSpace::Gamut::kRec2020_Gamut);

}
