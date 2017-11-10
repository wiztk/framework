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

#ifndef WIZTK_GUI_PUSH_BUTTON_HPP_
#define WIZTK_GUI_PUSH_BUTTON_HPP_

#include "abstract-button.hpp"
#include "wiztk/base/color.hpp"

namespace wiztk {
namespace gui {

/**
 * @ingroup gui
 * @brief A most commonly used button
 */
class PushButton : public AbstractButton {

 public:

  using ColorF = base::ColorF;

  explicit PushButton(const std::string &text);

 protected:

  virtual ~PushButton();

  void OnDraw(const Context &context) override;

 private:

  ColorF regular_;
  ColorF highlight_;
  ColorF active_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_PUSH_BUTTON_HPP_
