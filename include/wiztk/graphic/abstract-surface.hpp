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

#ifndef WIZTK_GRAPHIC_ABSTRACT_SURFACE_HPP_
#define WIZTK_GRAPHIC_ABSTRACT_SURFACE_HPP_

#include "wiztk/base/macros.hpp"
#include "wiztk/base/margin.hpp"
#include "wiztk/base/size.hpp"

namespace wiztk {
namespace graphic {

/**
 * @ingroup graphic
 * @brief Abstract surface for rendering 2D/3D contents.
 */
class WIZTK_EXPORT AbstractSurface {

 public:

  using Margin = base::Margin;
  using Size = base::SizeI;

  /**
   * @ingroup graphic
   * @brief A surface renderer
   */
  class WIZTK_EXPORT RenderDelegate {

   public:

    virtual ~RenderDelegate() = default;

    virtual void Render() {}

  };

  /**
   * @brief Default constructor
   */
  AbstractSurface() = default;

  /**
   * @brief Constructs a surface with given size and margin
   * @param size
   * @param margin
   */
  explicit AbstractSurface(const Size &size, const Margin &margin = Margin())
      : size_(size), margin_(margin) {}

  /**
   * @brief Constructs a surface with given size and margin
   * @param width
   * @param height
   * @param margin
   */
  AbstractSurface(int width, int height, const Margin &margin = Margin())
      : size_(width, height), margin_(margin) {}

  explicit AbstractSurface(const Margin &margin)
      : margin_(margin) {}

  /**
   * @brief Default copy constructor
   * @param other
   */
  AbstractSurface(const AbstractSurface &other) = default;

  /**
   * @brief Default move constructor
   * @param other
   */
  AbstractSurface(AbstractSurface &&other) = default;

  /**
   * @brief Default destructor
   */
  virtual ~AbstractSurface() = default;

  /**
   * @brief Default assignment operator
   * @param other
   * @return
   */
  AbstractSurface &operator=(const AbstractSurface &other) = default;

  /**
   * @brief Default move operator
   * @param other
   * @return
   */
  AbstractSurface &operator=(AbstractSurface &&other)= default;

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
  const Margin &margin() const { return margin_; }

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
  const Size &size() const { return size_; }

  /**
   * @brief Pure virtual method of how to render contents on this surface.
   * @param delegate
   */
  virtual void Render(RenderDelegate *delegate) = 0;

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
  virtual void OnResetMargin(int left, int top, int right, int bottom);

  /**
   * @brief Virtual method to handle resizing.
   * @param width Width
   * @param height Height
   *
   * If override this method in sub class, don't forget to call AbstractSurface::OnResize() to save.
   */
  virtual void OnResize(int width, int height);

 private:

  Size size_;
  Margin margin_;

};

} // namespace graphic
} // namespace wiztk

#endif // WIZTK_GRAPHIC_ABSTRACT_SURFACE_HPP_
