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

#ifndef WIZTK_GUI_PUSH_BUTTON_HPP_
#define WIZTK_GUI_PUSH_BUTTON_HPP_

#include "wiztk/base/color.hpp"
#include "wiztk/gui/abstract-button.hpp"

namespace wiztk {
namespace gui {

/**
 * @ingroup gui
 * @brief A most commonly used button
 */
class PushButton : public AbstractButton {

 public:

  using ColorF = base::ColorF;

  /**
   * @brief Create a new PushButton object.
   * @tparam Args
   * @param args
   * @return
   */
  template<typename ... Args>
  static PushButton *Create(Args &&... args);

 protected:

  explicit PushButton(const std::string &text);

  ~PushButton() override;

  void OnDraw(const Context &context) override;

 private:

  ColorF regular_;
  ColorF highlight_;
  ColorF active_;

};

template<typename ... Args>
PushButton *PushButton::Create(Args &&... args) {
  return new PushButton(std::forward<Args>(args)...);
}

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_PUSH_BUTTON_HPP_
