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

#ifndef WIZTK_GUI_INPUT_EVENT_HPP_
#define WIZTK_GUI_INPUT_EVENT_HPP_

#include "wiztk/base/macros.hpp"

namespace wiztk {
namespace gui {

class Input;
class Cursor;

WIZTK_EXPORT class InputEvent {

  friend class Input;
  friend class ViewSurface;

 public:

  enum Response {
    /**
     * @brief Unknown response
     */
        kUnknown,

    /**
     * @brief Accept this event and pass to next
     */
        kAccept,

    /**
     * @brief Ignore this event on this view and pass to next
     */
        kIgnore,

    /**
     * @brief Reject this event on this view and stop passing to next
     */
        kReject
  };

  InputEvent(Input *input)
      : input_(input),
        response_(kUnknown) {

  }

  virtual ~InputEvent() {}

  void SetCursor(const Cursor *cursor) const;

  void Accept() { response_ = kAccept; }

  void Ignore() { response_ = kIgnore; }

  void Reject() { response_ = kReject; }

  bool IsAccepted() const { return kAccept == response_; }

  bool IsIgnored() const { return kIgnore == response_; }

  bool IsRejected() const { return kReject == response_; }

 protected:

  Input *input() const { return input_; }

 private:

  Input *input_;

  Response response_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_INPUT_EVENT_HPP_
