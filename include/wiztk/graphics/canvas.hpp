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

#ifndef WIZTK_GRAPHICS_CANVAS_HPP_
#define WIZTK_GRAPHICS_CANVAS_HPP_

#include "wiztk/base/types.hpp"
#include "wiztk/base/size.hpp"
#include "wiztk/base/rect.hpp"
#include "wiztk/base/color.hpp"
#include "wiztk/base/macros.hpp"
#include "wiztk/base/deque.hpp"
#include "wiztk/base/string.hpp"

#include "wiztk/graphics/text-alignment.hpp"
#include "wiztk/graphics/clip-operation.hpp"

#include <memory>

namespace wiztk {
namespace graphics {

// Forward declarations:
class Image;
class Paint;
class Path;
class Matrix;
class Bitmap;
class ImageInfo;
class Surface;
class SurfaceProps;

/**
 * @ingroup graphics
 *
 * @brief A canvas is a container that holds various drawing elements
 *
 * A canvas is a container that holds various drawing elements (lines, shapes,
 * text, frames containing other elements, etc.). It takes its name from the
 * canvas used in visual arts. It arranges the logical representation of a user
 * interface or graphical scene.
 */
class Canvas {

  friend class Surface;

 public:

  /// @brief A type alias refers to base::RectF
  using RectF   = base::RectF;

  using RectI = base::RectI;

  /// @brief A type alias refers to base::String
  using String  = base::String;

  /// @brief A type alias refers to base::Point2F
  using Point2F = base::Point2F;

  class LockGuard;

  struct Private;

 public:

  WIZTK_DECLARE_NONCOPYABLE(Canvas);

  static Canvas *CreateRasterDirect(int width,
                                    int height,
                                    unsigned char *pixels,
                                    int format = kPixelFormatABGR8888);

  /**
   * @brief Create an empty canvas with no backing device/pixels, and zero dimentions
   */
  Canvas();

  Canvas(unsigned char *pixel, int width, int height,
         int format = kPixelFormatABGR8888);

  Canvas(int width, int height, const SurfaceProps *props = nullptr);

  explicit Canvas(const Bitmap &bitmap);

  Canvas(const Bitmap &bitmap, const SurfaceProps &props);

  Canvas(Canvas &&other) noexcept;

  virtual ~Canvas();

  Canvas &operator=(Canvas &&other) noexcept;

  void SetOrigin(float x, float y);

  /**
   * @brief Create a new surface matching the specified info
   * @param info
   * @return A pointer to a new created surface object, must be deleted manually
   */
  Surface MakeSurface(const ImageInfo &info, const SurfaceProps *props = nullptr);

  void DrawLine(float x0, float y0, float x1, float y1, const Paint &paint);

  void DrawRect(const RectF &rect, const Paint &paint);

  void DrawRoundRect(const RectF &rect, float rx, float ry, const Paint &paint);

  void DrawOval(const RectF &oval, const Paint &paint);

  void DrawCircle(float x, float y, float radius, const Paint &paint);

  void DrawArc(const RectF &oval, float start_angle, float sweep_angle,
               bool use_center, const Paint &paint);

  void DrawPath(const Path &path, const Paint &paint);

  void DrawText(const void *text, size_t byte_length, float x, float y, const Paint &paint,
                TextAlignment::Vertical vert = TextAlignment::kBaseline);

  void DrawText(const std::string &text, float x, float y, const Paint &paint,
                TextAlignment::Vertical vert = TextAlignment::kBaseline);

  void DrawText(const String &text, float x, float y, const Paint &paint,
                TextAlignment::Vertical vert = TextAlignment::kBaseline);

  void DrawImageRect(const Image &img, const RectF &src, const RectF &dst);

  void DrawPaint(const Paint &paint);

  void Translate(float dx, float dy);

  void Scale(float sx, float sy);

  void Rotate(float degrees);

  void Rotate(float degrees, float px, float py);

  void Skew(float sx, float sy);

  void Concat(const Matrix &matrix);

  void SetMatrix(const Matrix &matrix);

  /**
   * @brief Reset matrix and translate to origin
   */
  void ResetMatrix();

  void Clear(uint32_t argb = 0x0);

  void Clear(const base::ColorF &color);

  void ClipRect(const RectF &rect, ClipOperation op, bool antialias = false);

  void ClipRect(const RectF &rect, bool antialias = false);

  void ClipPath(const Path &path, ClipOperation op, bool antialias = false);

  void ClipPath(const Path &path, bool antialias = false);

  void Save();

  /**
   * @brief Save clip and matrix on stack, creates layer
   * @param bounds Hint to limit the size of the layer, may be nullptr
   * @param paint Graphics state for layer, may be nullptr
   */
  void SaveLayer(const RectF *bounds, const Paint *paint);

  /**
   * @brief Save clip and matrix on stack, create layer
   * @param bounds
   * @param alpha
   */
  void SaveLayer(const RectF *bounds, unsigned char alpha);

  void Restore();

  int GetSaveCount() const;

  void RestoreToCount(int save_count);

  void Flush();

  const Point2F &GetOrigin() const;

 private:

  struct LockGuardNode;

  explicit Canvas(Surface *surface);

  void DrawAlignedText(const void *text,
                       size_t byte_length,
                       float x,
                       float y,
                       const Paint &paint,
                       TextAlignment::Vertical vert = TextAlignment::kBaseline);

  std::unique_ptr<Private> p_;

};

struct Canvas::LockGuardNode : public base::DequeNode<Canvas::LockGuardNode> {

  LockGuardNode() = default;
  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(LockGuardNode);

  ~LockGuardNode() final = default;

  int depth = 0;

};

/**
 * @ingroup graphic
 * @brief A nested helper class to lock the status of a given canvas.
 */
class Canvas::LockGuard {

 public:

  LockGuard() = delete;
  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(LockGuard);

  /**
   * @brief Default constructor
   * @param canvas
   *
   * The default constructor does nothing, use a Lock() function after generation.
   */
  explicit LockGuard(Canvas *canvas)
      : canvas_(canvas) {}

  /**
   * @brief Generate a lockguard and clip the given rectangle
   * @param canvas
   * @param rect
   * @param antialias
   */
  LockGuard(Canvas *canvas, const RectF &rect, bool antialias = false)
      : canvas_(canvas) {
    Lock(rect, antialias);
  }

  /**
   * @brief Generate a lockguard and clip the given rectangle
   * @param canvas
   * @param rect
   * @param op
   * @param antialias
   */
  LockGuard(Canvas *canvas, const RectF &rect, ClipOperation op, bool antialias = false)
      : canvas_(canvas) {
    Lock(rect, op, antialias);
  }

  /**
   * @brief Generate a lockguard and clip the given path
   * @param canvas
   * @param path
   * @param antialias
   */
  LockGuard(Canvas *canvas, const Path &path, bool antialias = false)
      : canvas_(canvas) {
    Lock(path, antialias);
  }

  /**
   * @brief Generate a lockguard and clip the given path
   * @param canvas
   * @param path
   * @param op
   * @param antialias
   */
  LockGuard(Canvas *canvas, const Path &path, ClipOperation op, bool antialias = false)
      : canvas_(canvas) {
    Lock(path, op, antialias);
  }

  /**
   * @brief Generate a lockguard and translate
   * @param canvas
   * @param dx
   * @param dy
   */
  LockGuard(Canvas *canvas, float dx, float dy)
      : canvas_(canvas) {
    Lock(dx, dy);
  }

  ~LockGuard();

  void Lock(const RectF &rect, bool antilias = false);

  void Lock(const RectF &rect, ClipOperation op, bool antialias = false);

  void Lock(const Path &path, bool antialias = false);

  void Lock(const Path &path, ClipOperation op, bool antialias = false);

  /**
   * @brief Translate and lock
   * @param dx
   * @param dy
   */
  void Lock(float dx, float dy);

  void Unlock();

 private:

  Canvas *canvas_ = nullptr;

  LockGuardNode node_;

};

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHICS_CANVAS_HPP_
