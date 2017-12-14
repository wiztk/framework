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

#ifndef WIZTK_GUI_OUTPUT_HPP_
#define WIZTK_GUI_OUTPUT_HPP_

#include "wiztk/base/rect.hpp"
#include "wiztk/base/size.hpp"
#include "wiztk/base/sigcxx.hpp"
#include "wiztk/base/counted-deque.hpp"

#include <wayland-client.h>

#include <string>
#include <memory>

namespace wiztk {
namespace gui {

class Output : public base::CountedDeque::Element {

  friend class ViewSurface;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Output);

  template<typename ... Args> using SignalRefT = typename base::SignalRef<Args...>;
  template<typename ... Args> using SignalT = typename base::Signal<Args...>;

  Output(uint32_t id, uint32_t version);

  virtual ~Output();

  SignalRefT<Output *> destroyed() { return destroyed_; }

  int GetSubPixel() const;

  int GetTransform() const;

  int GetScale() const;

  const std::string &GetMake() const;

  const std::string &GetModel() const;

  uint32_t GetID() const;

  uint32_t GetVersion() const;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

  SignalT<Output *> destroyed_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_OUTPUT_HPP_
