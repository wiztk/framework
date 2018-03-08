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

#ifndef WIZTK_GUI_CALLBACK_HPP_
#define WIZTK_GUI_CALLBACK_HPP_

#include "wiztk/base/delegate.hpp"
#include "wiztk/base/macros.hpp"

#include <memory>

namespace wiztk {
namespace gui {

class Display;
class ViewSurface;

/**
 * @ingroup gui
 * @brief Callback class in gui module
 */
class Callback {

 public:

  // Declarations

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Callback);

  template<typename R, typename ... P> using DelegateRef = typename base::DelegateRef<R, P...>;
  template<typename R, typename ... P> using Delegate = typename base::Delegate<R, P...>;

 public:

  Callback();

  explicit Callback(const Display *display);

  explicit Callback(const ViewSurface *surface);

  ~Callback();

  void Setup(const Display *display);

  void Setup(const ViewSurface *surface);

  /**
   * @brief A delegate to the 'done' event
   */
  DelegateRef<void(uint32_t)> done() { return done_; }

 private:

  struct Private;

  std::unique_ptr<Private> p_;

  Delegate<void(uint32_t)> done_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_CALLBACK_HPP_
