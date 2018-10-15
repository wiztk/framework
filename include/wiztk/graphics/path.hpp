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

#ifndef WIZTK_GRAPHIC_PATH_HPP_
#define WIZTK_GRAPHIC_PATH_HPP_

#include "wiztk/base/rect.hpp"

#include <memory>

namespace wiztk {
namespace graphics {

/**
 * @ingroup graphics
 * @brief Path
 */
class Path {

  friend bool operator==(const Path &, const Path &);
  friend bool operator!=(const Path &, const Path &);

 public:

  struct Private;

  enum Direction {
    kClockwise,
    kCounterClockwise
  };

  enum FillType {
    kFillTypeWinding,
    kFillTypeEvenOdd,
    kFillTypeInverseWinding,
    kFillTypeInverseEvenOdd
  };

  enum Convexity {
    kConvexityUnknown,
    kConvexityConvex,
    kConvexityConcave
  };

  Path();

  Path(const Path &other);

  ~Path();

  Path &operator=(const Path &other);

  bool IsInterpolatable(const Path &compare) const;

  bool Interpolate(const Path &ending, float weight, Path *out) const;

  void AddRect(const base::RectF &rect, Direction dir = kClockwise);

  void AddRect(const base::RectF &rect, Direction dir, unsigned start);

  void AddRect(float left, float top, float right, float bottom, Direction dir = kClockwise);

  void AddRoundRect(const base::RectF &rect, const float radii[], Direction dir = kClockwise);

  FillType GetFillType() const;

  void SetFillType(FillType fill_type);

  bool IsInverseFillType() const;

  void ToggleInverseFillType();

  Convexity GetConvexity() const;

  Convexity GetConvexityOrUnknown() const;

  void SetConvexity(Convexity convexity);

  bool IsConvex() const;

  void Reset();

  void Rewind();

  bool IsEmpty() const;

  int CountPoints() const;

  base::Point2F GetPoint(int index) const;

  int GetPoints(base::Point2F points[], int max) const;

  int CountVerbs() const;

  void Swap(Path &other);

  const base::RectF &GetBounds() const;

  void UpdateBoundsCache() const;

  void MoveTo(float x, float y);

  void MoveTo(const base::Point2F &p);

  void RelativeMoveTo(float dx, float dy);

  void LineTo(float x, float y);

  void LineTo(const base::Point2F &p);

  void RelativeLineTo(float dx, float dy);

  void QuadTo(float x1, float y1, float x2, float y2);

  void QuadTo(const base::Point2F &p1, const base::Point2F &p2);

  void RelativeQuadTo(float dx1, float dy1, float dx2, float dy2);

  void Close();

 private:

  std::unique_ptr<Private> p_;

};

bool operator==(const Path &path1, const Path &path2);

bool operator!=(const Path &path1, const Path &path2);

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHIC_PATH_HPP_
