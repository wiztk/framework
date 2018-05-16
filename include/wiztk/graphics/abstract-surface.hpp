/*
 * Copyright 2017 The WizTK Authors.
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

/**
 * @file abstract-surface.hpp
 * @brief Header file for abstract class of drawable surface object.
 */

#ifndef WIZTK_GRAPHICS_ABSTRACT_SURFACE_HPP_
#define WIZTK_GRAPHICS_ABSTRACT_SURFACE_HPP_

#include "wiztk/base/macros.hpp"
#include "wiztk/base/thickness.hpp"
#include "wiztk/base/size.hpp"

#include <memory>

namespace wiztk {
namespace graphics {

/**
 * @ingroup graphic
 * @brief Abstract surface for rendering 2D/3D contents.
 */
class WIZTK_EXPORT AbstractSurface {

 public:

  using Margin = base::ThicknessI;
  using Size = base::SizeI;

  /**
   * @ingroup graphic
   * @brief A delegate to render surface, used when calling AbstractSurface::Render()
   */
  class WIZTK_EXPORT Delegate {

   public:

    Delegate() = default;

    virtual ~Delegate() = default;

    virtual void Render() {}

  };

  /**
   * @brief Default constructor
   */
  AbstractSurface();

  /**
   * @brief Constructs a surface with given size and margin
   * @param size
   * @param margin
   */
  explicit AbstractSurface(const Size &size, const Margin &margin = Margin());

  /**
   * @brief Constructs a surface with given size and margin
   * @param width
   * @param height
   * @param margin
   */
  AbstractSurface(int width, int height, const Margin &margin = Margin());

  explicit AbstractSurface(const Margin &margin);

  /**
   * @brief Default move constructor
   * @param other
   */
  AbstractSurface(AbstractSurface &&other) noexcept;

  /**
   * @brief Default destructor
   */
  virtual ~AbstractSurface();

  /**
   * @brief Default move operator
   * @param other
   * @return
   */
  AbstractSurface &operator=(AbstractSurface &&other) noexcept;

  /**
   * @brief Call OnSetMargin()
   * @param margin
   */
  void SetMargin(const Margin &margin);

  /**
   * @brief Call OnSetMargin()
   * @param left
   * @param top
   * @param right
   * @param bottom
   */
  void SetMargin(int left, int top, int right, int bottom);

  /**
   * @brief Get the margin
   * @return
   */
  const Margin &GetMargin() const;

  /**
   * @brief Resize
   * @param size
   */
  void Resize(const Size &size);

  /**
   * @brief Resize
   * @param width
   * @param height
   */
  void Resize(int width, int height);

  /**
   * @brief Get size
   * @return
   */
  const Size &GetSize() const;

  /**
   * @brief Pure virtual method of how to render contents on this surface.
   * @param delegate
   */
  virtual void Render(Delegate *delegate) = 0;

 protected:

  /**
   * @brief Virtual method to handle margin resetting.
   * @param left Left
   * @param top Top
   * @param right Right
   * @param bottom Bottom
   *
   * If override this method in sub class, don't forget to call AbstractSurface::OnSetMargin() to save.
   */
  virtual bool OnResetMargin(int left, int top, int right, int bottom) = 0;

  /**
   * @brief Virtual method to handle resizing.
   * @param width Width
   * @param height Height
   *
   * If override this method in sub class, don't forget to call AbstractSurface::OnResize() to save.
   */
  virtual bool OnResize(int width, int height) = 0;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

} // namespace graphics
} // namespace wiztk

#endif // WIZTK_GRAPHICS_ABSTRACT_SURFACE_HPP_
