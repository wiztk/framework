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

#ifndef WIZTK_GUI_ABSTRACT_LAYOUT_HPP_
#define WIZTK_GUI_ABSTRACT_LAYOUT_HPP_

#include "abstract-view.hpp"

namespace wiztk {
namespace gui {

/**
 * @ingroup gui
 * @brief The base abstract layout class
 */
WIZTK_EXPORT class AbstractLayout : public AbstractView {

  friend class AbstractView;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(AbstractLayout);

  AbstractLayout(const Padding &padding = Padding(5));

  void AddView(AbstractView *view);

  void AddView(int index, AbstractView *view);

  void AddView(int row, int column, AbstractView *view);

  void RemoveView(AbstractView *view);

  void Layout();

 protected:

  virtual ~AbstractLayout();

  virtual void OnConfigureGeometry(const RectF &old_geometry,
                                   const RectF &new_geometry) final;

  virtual void OnSaveGeometry(const RectF &old_geometry,
                              const RectF &new_geometry) final;

  virtual void OnRequestUpdateFrom(AbstractView *view) override;

  virtual void OnChildAdded(AbstractView *view) final;

  virtual void OnChildRemoved(AbstractView *view) final;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave() override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseDown(MouseEvent *event) override;

  virtual void OnMouseUp(MouseEvent *event) override;

  virtual void OnKeyDown(KeyEvent *event) override;

  virtual void OnKeyUp(KeyEvent *event) override;

  virtual void OnDraw(const Context &context) override;

  virtual void OnViewAdded(AbstractView *view) = 0;

  virtual void OnViewRemoved(AbstractView *view) = 0;

  virtual void OnLayout(int left, int top, int right, int bottom) = 0;

 private:

  struct Private;

  bool is_geometry_saved_;

//  std::unique_ptr<Private> p_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_ABSTRACT_LAYOUT_HPP_
