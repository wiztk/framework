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

#ifndef WIZTK_GUI_DIALOG_HPP_
#define WIZTK_GUI_DIALOG_HPP_

#include "abstract-shell-view.hpp"

#include <memory>

namespace wiztk {
namespace gui {

/**
 * @ingroup gui
 * @brief This class is currently for test
 */
WIZTK_EXPORT class Dialog : public AbstractShellView {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Dialog);
  Dialog() = delete;

  explicit Dialog(const char *title, AbstractShellView *parent = nullptr);

  virtual ~Dialog();

 protected:

  virtual void OnShown() final;

  virtual void OnConfigureSize(const Size &old_size, const Size &new_size) final;

  virtual void OnSaveSize(const Size &old_size, const Size &new_size) final;

  virtual void OnRenderSurface(ViewSurface *surface) final;

 private:

  struct Private;

  void DrawFrame(const Context &context);

  std::unique_ptr<Private> p_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_DIALOG_HPP_
