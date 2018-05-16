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

#ifndef WIZTK_GUI_OUTPUT_MANAGER_HPP_
#define WIZTK_GUI_OUTPUT_MANAGER_HPP_

#include "wiztk/base/counted-deque.hpp"

#include "wiztk/gui/output.hpp"

namespace wiztk {
namespace gui {

/**
 * @ingroup gui
 * @brief Singleton output manager controlled by Display.
 *
 * The OutputManager is created when connecting a client display and destroyed
 * when disconnecting. To get the singleton instance, use the method in Display,
 * for example:
 *
 * @code
 * #include "wiztk/gui/application.hpp"
 * #include "wiztk/gui/display.hpp"
 *
 * // ...
 *
 * Display* display = Application::GetInstance()->GetDisplay();
 * InputManager *input_mgr = display->GetOutputManager();
 * @endcode
 */
class WIZTK_EXPORT OutputManager {

  friend class Display;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(OutputManager);

  template<typename ... Args> using SignalRef = typename base::SignalRef<Args...>;
  template<typename ... Args> using Signal = typename base::Signal<Args...>;

 public:

  Output *GetActiveOutput() const;

  size_t size() const { return deque_.count(); }

  Output *FindByID(uint32_t id) const;

  SignalRef<Output *> added() { return added_; }

  SignalRef<Output *> removed() { return removed_; }

 private:

  typedef base::CountedDeque<Output::Private> OutputPrivateDeque;

  /**
   * @brief Default constructor.
   */
  OutputManager() = default;

  /**
   * @brief Destructor.
   */
  ~OutputManager();

  /**
   * @brief Create and store a new Output.
   * @param id
   * @param version
   *
   * This private method is only called in Display once a new wayland output is registered.
   */
  void AddOutput(uint32_t id, uint32_t version);

  /**
   * @brief Remove and distroy an Output.
   * @param id
   *
   * This private method is only called in Display once an wayland output is removed.
   */
  void RemoveOutput(uint32_t id);

  void Clear();

  OutputPrivateDeque deque_;

  Signal<Output *> added_;

  Signal<Output *> removed_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_OUTPUT_MANAGER_HPP_
