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

#ifndef WIZTK_GUI_GL_WIDGET_HPP_
#define WIZTK_GUI_GL_WIDGET_HPP_

#include "abstract-view.hpp"

namespace wiztk {
namespace gui {

class Surface;
class AbstractRenderingAPI;

class GLView : public AbstractView {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(GLView);

  GLView();

  void SetRenderingAPI(AbstractRenderingAPI *api);

 protected:

  virtual ~GLView();

  void OnConfigureGeometry(const RectF &old_geometry, const RectF &new_geometry) final;

  void OnSaveGeometry(const RectF &old_geometry, const RectF &new_geometry) final;

  void OnMouseEnter(MouseEvent *event) override;

  void OnMouseLeave() override;

  void OnMouseMove(MouseEvent *event) override;

  void OnMouseDown(MouseEvent *event) override;

  void OnMouseUp(MouseEvent *event) override;

  void OnKeyDown(KeyEvent *event) override;

  void OnKeyUp(KeyEvent *event) override;

  void OnDraw(const Context &context) final;

  void OnRenderSurface(Surface *surface) override;

  virtual void OnInitialize() = 0;

  virtual void OnResize(int width, int height) = 0;

  virtual void OnRender() = 0;

  void SwapBuffers();

  void MakeCurrent();

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_GL_WIDGET_HPP_
