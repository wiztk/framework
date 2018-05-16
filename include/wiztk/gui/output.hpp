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

#ifndef WIZTK_GUI_OUTPUT_HPP_
#define WIZTK_GUI_OUTPUT_HPP_

#include "wiztk/base/rect.hpp"
#include "wiztk/base/size.hpp"
#include "wiztk/base/sigcxx.hpp"
#include "wiztk/base/counted-deque.hpp"

#include <string>
#include <memory>

namespace wiztk {
namespace gui {

/**
 * @ingroup gui
 * @brief Display output.
 */
class Output {

  friend class ViewSurface;
  friend class OutputManager;

 public:

  /** @brief Declare non-copyable and non-movable */
  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Output);

  /** @brief Disable the default constructor */
  Output() = delete;

  /** @brief Alias name for short. */
  template<typename ... Args> using SignalRef = typename base::SignalRef<Args...>;

  /** @brief Alias name for short. */
  template<typename ... Args> using Signal = typename base::Signal<Args...>;

 public:

  int GetSubPixel() const;

  int GetTransform() const;

  int GetScale() const;

  const std::string &GetMake() const;

  const std::string &GetModel() const;

  uint32_t GetID() const;

  uint32_t GetVersion() const;

  SignalRef<Output *> destroyed() { return destroyed_; }

 private:

  struct Private;

  Output(uint32_t id, uint32_t version);

  ~Output();

  std::unique_ptr<Private> p_;

  Signal<Output *> destroyed_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_OUTPUT_HPP_
