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

#include "image-info-test.hpp"

#include <wiztk/graphics/image-info.hpp>

#include "SkImageInfo.h"

using namespace wiztk::graphics;

TEST_F(ImageInfoTest, enum_alpha_type) {

  ASSERT_TRUE((int) AlphaType::kAlphaTypeUnknown == (int) SkAlphaType::kUnknown_SkAlphaType);
  ASSERT_TRUE((int) AlphaType::kAlphaTypeOpaque == (int) SkAlphaType::kOpaque_SkAlphaType);
  ASSERT_TRUE((int) AlphaType::kAlphaTypePremul == (int) SkAlphaType::kPremul_SkAlphaType);
  ASSERT_TRUE((int) AlphaType::kAlphaTypeUnpremul == (int) SkAlphaType::kUnpremul_SkAlphaType);
  ASSERT_TRUE((int) AlphaType::kAlphaTypeLast == (int) SkAlphaType::kLastEnum_SkAlphaType);

}

TEST_F(ImageInfoTest, enum_color_type) {

  ASSERT_TRUE((int) ColorType::kColorTypeUnknown == (int) SkColorType::kUnknown_SkColorType);
  ASSERT_TRUE((int) ColorType::kColorTypeAlpha8 == (int) SkColorType::kAlpha_8_SkColorType);
  ASSERT_TRUE((int) ColorType::kColorTypeRGB565 == (int) SkColorType::kRGB_565_SkColorType);
  ASSERT_TRUE((int) ColorType::kColorTypeARGB4444 == (int) SkColorType::kARGB_4444_SkColorType);
  ASSERT_TRUE((int) ColorType::kColorTypeRGBA8888 == (int) SkColorType::kRGBA_8888_SkColorType);
  ASSERT_TRUE((int) ColorType::kColorTypeRGB888x == (int) SkColorType::kRGB_888x_SkColorType);
  ASSERT_TRUE((int) ColorType::kColorTypeBGRA8888 == (int) SkColorType::kBGRA_8888_SkColorType);
  ASSERT_TRUE((int) ColorType::kColorTypeRGBA1010102 == (int) SkColorType::kRGBA_1010102_SkColorType);
  ASSERT_TRUE((int) ColorType::kColorTypeRGB101010x == (int) SkColorType::kRGB_101010x_SkColorType);
  ASSERT_TRUE((int) ColorType::kColorTypeGray8 == (int) SkColorType::kGray_8_SkColorType);
  ASSERT_TRUE((int) ColorType::kColorTypeRGBAF16 == (int) SkColorType::kRGBA_F16_SkColorType);
  ASSERT_TRUE((int) ColorType::kColorTypeRGBAF32 == (int) SkColorType::kRGBA_F32_SkColorType);
  ASSERT_TRUE((int) ColorType::kColorTypeLast == (int) SkColorType::kLastEnum_SkColorType);

}
