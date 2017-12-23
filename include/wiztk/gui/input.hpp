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

#ifndef WIZTK_GUI_INPUT_HPP_
#define WIZTK_GUI_INPUT_HPP_

#include "wiztk/base/counted-deque.hpp"

#include <memory>

namespace wiztk {
namespace gui {

class Cursor;
class KeyEvent;
class MouseEvent;
class TouchEvent;
class AbstractView;
struct ViewTask;

/**
 * @ingroup gui
 * @brief Input manager
 */
class WIZTK_EXPORT Input : public base::CountedDequeNode<Input> {

  friend class ViewSurface;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Input);
  Input() = delete;

  Input(uint32_t id, uint32_t version);

  ~Input() final;

  void SetCursor(const Cursor *cursor) const;

  uint32_t GetID() const;

  uint32_t GetVersion() const;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_INPUT_HPP_
