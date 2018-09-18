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

#ifndef WIZTK_BASE_TYPES_HPP_
#define WIZTK_BASE_TYPES_HPP_

#include <cstdint>

namespace wiztk {

enum CursorType {
  kCursorBottomLeft,
  kCursorBottomRight,
  kCursorBottom,
  kCursorDragging,
  kCursorLeftPtr,
  kCursorLeft,
  kCursorRight,
  kCursorTopLeft,
  kCursorTopRight,
  kCursorTop,
  kCursorIbeam,
  kCursorHand1,
  kCursorWatch,
  kCursorDndMove,
  kCursorDndCopy,
  kCursorDndForbidden,

  kCursorBlank
};

enum PixelFormat {

  /**
   * -1 No such format exists or is supported
   */
  kPixelFormatInvalid = -1,

  /**
   * 32-bit ARGB format, [31:0] A:R:G:B 8:8:8:8 little endian
   */
  kPixelFormatARGB8888 = 0,  //  WL_SHM_FORMAT_ARGB8888
  
  /**
   * 32-bit RGB format, [31:0] x:R:G:B 8:8:8:8 little endian
   */
  kPixelFormatXRGB8888 = 1, //  WL_SHM_FORMAT_XRGB8888
  
  /**
   * 8-bit color index format, [7:0] C
   */
  kPixelFormatC8 = 0x20203843,  // WL_SHM_FORMAT_C8
  
  /**
   * 8-bit RGB format, [7:0] R:G:B 3:3:2
   */
  kPixelFormatRGB332 = 0x38424752,  // WL_SHM_FORMAT_RGB332
  
  /**
   * 8-bit BGR format, [7:0] B:G:R 2:3:3
   */
  kPixelFormatBGR233 = 0x38524742,  // WL_SHM_FORMAT_BGR233
  
  /**
   * 16-bit xRGB format, [15:0] x:R:G:B 4:4:4:4 little endian
   */
  kPixelFormatXRGB4444 = 0x32315258, // WL_SHM_FORMAT_XRGB4444
  
  /**
   * 16-bit xBGR format, [15:0] x:B:G:R 4:4:4:4 little endian
   */
  kPixelFormatXBGR4444 = 0x32314258,  // WL_SHM_FORMAT_XBGR4444
  
  /**
   * 16-bit RGBx format, [15:0] R:G:B:x 4:4:4:4 little endian
   */
  kPixelFormatRGBX4444 = 0x32315852,  // WL_SHM_FORMAT_RGBX4444
  
  /**
   * 16-bit BGRx format, [15:0] B:G:R:x 4:4:4:4 little endian
   */
  kPixelFormatBGRX4444 = 0x32315842,  // WL_SHM_FORMAT_BGRX4444
  
  /**
   * 16-bit ARGB format, [15:0] A:R:G:B 4:4:4:4 little endian
   */
  kPixelFormatARGB4444 = 0x32315241,  // WL_SHM_FORMAT_ARGB4444
  
  /**
   * 16-bit ABGR format, [15:0] A:B:G:R 4:4:4:4 little endian
   */
  kPixelFormatABGR4444 = 0x32314241,  // WL_SHM_FORMAT_ABGR4444
  
  /**
   * 16-bit RBGA format, [15:0] R:G:B:A 4:4:4:4 little endian
   */
  kPixelFormatRGBA4444 = 0x32314152,  // WL_SHM_FORMAT_RGBA4444
  
  /**
   * 16-bit BGRA format, [15:0] B:G:R:A 4:4:4:4 little endian
   */
  kPixelFormatBGRA4444 = 0x32314142,  // WL_SHM_FORMAT_BGRA4444
  
  /**
   * 16-bit xRGB format, [15:0] x:R:G:B 1:5:5:5 little endian
   */
  kPixelFormatXRGB1555 = 0x35315258,  // WL_SHM_FORMAT_XRGB1555
  
  /**
   * 16-bit xBGR 1555 format, [15:0] x:B:G:R 1:5:5:5 little endian
   */
  kPixelFormatXBGR1555 = 0x35314258,  // WL_SHM_FORMAT_XBGR1555
  
  /**
   * 16-bit RGBx 5551 format, [15:0] R:G:B:x 5:5:5:1 little endian
   */
  kPixelFormatRGBX5551 = 0x35315852,  // WL_SHM_FORMAT_RGBX5551
  
  /**
   * 16-bit BGRx 5551 format, [15:0] B:G:R:x 5:5:5:1 little endian
   */
  kPixelFormatBGRX5551 = 0x35315842,  // WL_SHM_FORMAT_BGRX5551
  
  /**
   * 16-bit ARGB 1555 format, [15:0] A:R:G:B 1:5:5:5 little endian
   */
  kPixelFormatARGB1555 = 0x35315241,  // WL_SHM_FORMAT_ARGB1555
  
  /**
   * 16-bit ABGR 1555 format, [15:0] A:B:G:R 1:5:5:5 little endian
   */
  kPixelFormatABGR1555 = 0x35314241,  // WL_SHM_FORMAT_ABGR1555
  
  /**
   * 16-bit RGBA 5551 format, [15:0] R:G:B:A 5:5:5:1 little endian
   */
  kPixelFormatRGBA5551 = 0x35314152,  // WL_SHM_FORMAT_RGBA5551
  
  /**
   * 16-bit BGRA 5551 format, [15:0] B:G:R:A 5:5:5:1 little endian
   */
  kPixelFormatBGRA5551 = 0x35314142,  // WL_SHM_FORMAT_BGRA5551
  
  /**
   * 16-bit RGB 565 format, [15:0] R:G:B 5:6:5 little endian
   */
  kPixelFormatRGB565 = 0x36314752,  // WL_SHM_FORMAT_RGB565
  
  /**
   * 16-bit BGR 565 format, [15:0] B:G:R 5:6:5 little endian
   */
  kPixelFormatBGR565 = 0x36314742,  // WL_SHM_FORMAT_BGR565
  
  /**
   * 24-bit RGB format, [23:0] R:G:B little endian
   */
  kPixelFormatRGB888 = 0x34324752,  // WL_SHM_FORMAT_RGB888
  
  /**
   * 24-bit BGR format, [23:0] B:G:R little endian
   */
  kPixelFormatBGR888 = 0x34324742,  // WL_SHM_FORMAT_BGR888
  
  /**
   * 32-bit xBGR format, [31:0] x:B:G:R 8:8:8:8 little endian
   */
  kPixelFormatXBGR8888 = 0x34324258,  // WL_SHM_FORMAT_XBGR8888
  
  /**
   * 32-bit RGBx format, [31:0] R:G:B:x 8:8:8:8 little endian
   */
  kPixelFormatRGBX8888 = 0x34325852,  // WL_SHM_FORMAT_RGBX8888
  
  /**
   * 32-bit BGRx format, [31:0] B:G:R:x 8:8:8:8 little endian
   */
  kPixelFormatBGRX8888 = 0x34325842,  // WL_SHM_FORMAT_BGRX8888
  
  /**
   * 32-bit ABGR format, [31:0] A:B:G:R 8:8:8:8 little endian
   */
  kPixelFormatABGR8888 = 0x34324241,  // WL_SHM_FORMAT_ABGR8888
  
  /**
   * 32-bit RGBA format, [31:0] R:G:B:A 8:8:8:8 little endian
   */
  kPixelFormatRGBA8888 = 0x34324152,  // WL_SHM_FORMAT_RGBA8888
  
  /**
   * 32-bit BGRA format, [31:0] B:G:R:A 8:8:8:8 little endian
   */
  kPixelFormatBGRA8888 = 0x34324142,  // WL_SHM_FORMAT_BGRA8888
  
  /**
   * 32-bit xRGB format, [31:0] x:R:G:B 2:10:10:10 little endian
   */
  kPixelFormatXRGB2101010 = 0x30335258, // WL_SHM_FORMAT_XRGB2101010
  
  /**
   * 32-bit xBGR format, [31:0] x:B:G:R 2:10:10:10 little endian
   */
  kPixelFormatXBGR2101010 = 0x30334258, // WL_SHM_FORMAT_XBGR2101010
  
  /**
   * 32-bit RGBx format, [31:0] R:G:B:x 10:10:10:2 little endian
   */
  kPixelFormatRGBX1010102 = 0x30335852, // WL_SHM_FORMAT_RGBX1010102
  
  /**
   * 32-bit BGRx format, [31:0] B:G:R:x 10:10:10:2 little endian
   */
  kPixelFormatBGRX1010102 = 0x30335842, // WL_SHM_FORMAT_BGRX1010102
  
  /**
   * 32-bit ARGB format, [31:0] A:R:G:B 2:10:10:10 little endian
   */
  kPixelFormatARGB2101010 = 0x30335241, // WL_SHM_FORMAT_ARGB2101010
  
  /**
   * 32-bit ABGR format, [31:0] A:B:G:R 2:10:10:10 little endian
   */
  kPixelFormatABGR2101010 = 0x30334241, // WL_SHM_FORMAT_ABGR2101010
  
  /**
   * 32-bit RGBA format, [31:0] R:G:B:A 10:10:10:2 little endian
   */
  kPixelFormatRGBA1010102 = 0x30334152, // WL_SHM_FORMAT_RGBA1010102
  
  /**
   * 32-bit BGRA format, [31:0] B:G:R:A 10:10:10:2 little endian
   */
  kPixelFormatBGRA1010102 = 0x30334142, // WL_SHM_FORMAT_BGRA1010102
  
  /**
   * packed YCbCr format, [31:0] Cr0:Y1:Cb0:Y0 8:8:8:8 little endian
   */
  kPixelFormatYUYV = 0x56595559,  // WL_SHM_FORMAT_YUYV
  
  /**
   * packed YCbCr format, [31:0] Cb0:Y1:Cr0:Y0 8:8:8:8 little endian
   */
  kPixelFormatYVYU = 0x55595659,  // WL_SHM_FORMAT_YVYU
  
  /**
   * packed YCbCr format, [31:0] Y1:Cr0:Y0:Cb0 8:8:8:8 little endian
   */
  kPixelFormatUYVY = 0x59565955,  // WL_SHM_FORMAT_UYVY
  
  /**
   * packed YCbCr format, [31:0] Y1:Cb0:Y0:Cr0 8:8:8:8 little endian
   */
  kPixelFormatVYUY = 0x59555956,  // WL_SHM_FORMAT_VYUY
  
  /**
   * packed AYCbCr format, [31:0] A:Y:Cb:Cr 8:8:8:8 little endian
   */
  kPixelFormatAYUV = 0x56555941,  // WL_SHM_FORMAT_AYUV
  
  /**
   * 2 plane YCbCr Cr:Cb format, 2x2 subsampled Cr:Cb plane
   */
  kPixelFormatNV12 = 0x3231564e,  // WL_SHM_FORMAT_NV12

  /**
   * 2 plane YCbCr Cb:Cr format, 2x2 subsampled Cb:Cr plane
   */
  kPixelFormatNV21 = 0x3132564e,  // WL_SHM_FORMAT_NV21

  /**
   * 2 plane YCbCr Cr:Cb format, 2x1 subsampled Cr:Cb plane
   */
  kPixelFormatNV16 = 0x3631564e,  // WL_SHM_FORMAT_NV16
  
  /**
   * 2 plane YCbCr Cb:Cr format, 2x1 subsampled Cb:Cr plane
   */
  kPixelFormatNV61 = 0x3136564e,  // WL_SHM_FORMAT_NV61
  
  /**
   * 3 plane YCbCr format, 4x4 subsampled Cb (1) and Cr (2) planes
   */
  kPixelFormatYUV410 = 0x39565559,  // WL_SHM_FORMAT_YUV410
  
  /**
   * 3 plane YCbCr format, 4x4 subsampled Cr (1) and Cb (2) planes
   */
  kPixelFormatYVU410 = 0x39555659,  // WL_SHM_FORMAT_YVU410

  /**
   * 3 plane YCbCr format, 4x1 subsampled Cb (1) and Cr (2) planes
   */
  kPixelFormatYUV411 = 0x31315559,  // WL_SHM_FORMAT_YUV411

  /**
   * 3 plane YCbCr format, 4x1 subsampled Cr (1) and Cb (2) planes
   */
  kPixelFormatYVU411 = 0x31315659,  // WL_SHM_FORMAT_YVU411

  /**
   * 3 plane YCbCr format, 2x2 subsampled Cb (1) and Cr (2) planes
   */
  kPixelFormatYUV420 = 0x32315559,  // WL_SHM_FORMAT_YUV420

  /**
   * 3 plane YCbCr format, 2x2 subsampled Cr (1) and Cb (2) planes
   */
  kPixelFormatYVU420 = 0x32315659,  // WL_SHM_FORMAT_YVU420

  /**
   * 3 plane YCbCr format, 2x1 subsampled Cb (1) and Cr (2) planes
   */
  kPixelFormatYUV422 = 0x36315559,  // WL_SHM_FORMAT_YUV422

  /**
   * 3 plane YCbCr format, 2x1 subsampled Cr (1) and Cb (2) planes
   */
  kPixelFormatYVU422 = 0x36315659,  // WL_SHM_FORMAT_YVU422

  /**
   * 3 plane YCbCr format, non-subsampled Cb (1) and Cr (2) planes
   */
  kPixelFormatYUV444 = 0x34325559,  // WL_SHM_FORMAT_YUV444

  /**
   * 3 plane YCbCr format, non-subsampled Cr (1) and Cb (2) planes
   */
  kPixelFormatYVU444 = 0x34325659  // WL_SHM_FORMAT_YVU444
};

enum Orientation {
  kHorizontal,
  kVertical
};

typedef uint16_t GlyphID;

}

#endif // WIZTK_BASE_TYPES_HPP_
