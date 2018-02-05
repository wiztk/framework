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

#include "bitmap_private.hpp"
#include "image-info_private.hpp"

#include "OpenImageIO/imageio.h"
#include "OpenImageIO/imagebufalgo.h"
#include "OpenImageIO/imagebuf.h"

namespace wiztk {
namespace graphic {

Bitmap::Bitmap() {
  p_ = std::make_unique<Private>(this);
}

Bitmap::Bitmap(const Bitmap &orig) {
  p_ = std::make_unique<Private>(this);

  p_->sk_bitmap = orig.p_->sk_bitmap;
}

Bitmap &Bitmap::operator=(const Bitmap &other) {
  p_->sk_bitmap = other.p_->sk_bitmap;
  return *this;
}

Bitmap::~Bitmap() {

}

void Bitmap::AllocatePixels(const ImageInfo &info) {
  p_->sk_bitmap.allocPixels(SkImageInfo::Make(info.width(),
                                              info.height(),
                                              static_cast<SkColorType>(info.color_type()),
                                              static_cast<SkAlphaType>(info.alpha_type())));
}

void Bitmap::AllocateN32Pixels(int width, int height, bool is_opaque) {
  p_->sk_bitmap.allocN32Pixels(width, height, is_opaque);
}

bool Bitmap::InstallPixels(const ImageInfo &info, void *pixels, size_t row_bytes) {
  return p_->sk_bitmap.installPixels(SkImageInfo::Make(info.width(),
                                                       info.height(),
                                                       static_cast<SkColorType >(info.color_type()),
                                                       static_cast<SkAlphaType >(info.alpha_type())),
                                     pixels,
                                     row_bytes);
}

void Bitmap::WriteToFile(const std::string &filename) const {
  using OIIO::ImageOutput;
  using OIIO::ImageSpec;
  using OIIO::TypeDesc;
  using OIIO::ImageBuf;
  using namespace OIIO::ImageBufAlgo;

  ImageOutput *out = ImageOutput::create(filename);
  if (nullptr == out) return;

  TypeDesc::BASETYPE base_type = TypeDesc::UNKNOWN;
  switch (p_->sk_bitmap.bytesPerPixel()) {
    // TODO: other color type
    case 4: {
      base_type = TypeDesc::UINT8;
      break;
    }
    default:break;
  }

  if (TypeDesc::UNKNOWN == base_type) {
    fprintf(stderr, "Unkown image type!\n");
    return;
  }

  // TODO: determine the number of channel

  ImageSpec spec(p_->sk_bitmap.width(), p_->sk_bitmap.height(), 4, base_type);

  // Use ImageBuf to convert BGRA to RGBA
  ImageBuf bgra(spec, p_->sk_bitmap.getPixels());
  ImageBuf rgba;

  std::vector<int> channel_order = {2, 1, 0, 3};
  channels(rgba, bgra, 4, channel_order.data());

  out->open(filename, spec);
//  out->write_image(TypeDesc::UINT8, p_->sk_bitmap.getPixels());

  rgba.write(out);

  out->close();
  ImageOutput::destroy(out);
}

int Bitmap::GetWidth() const {
  return p_->sk_bitmap.width();
}

int Bitmap::GetHeight() const {
  return p_->sk_bitmap.height();
}

ColorType Bitmap::GetColorType() const {
  return static_cast<ColorType>(p_->sk_bitmap.colorType());
}

AlphaType Bitmap::GetAlphaType() const {
  return static_cast<AlphaType>(p_->sk_bitmap.alphaType());
}

} // namespace graphic
} // namespace wiztk
