/*
 * Copyright 2017 The WizTK Authors.
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
 * @brief Output manager controlled by Display.
 */
class OutputManager {

  friend class Display;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(OutputManager);

  typedef base::CountedDeque<Output::Private> OutputPrivateDeque;

 public:

  /**
   * @brief Default constructor.
   */
  OutputManager() = default;

  /**
   * @brief Destructor.
   */
  ~OutputManager();

  /**
   * @brief Add a output.
   * @param output
   */
  void AddOutput(Output *output);

  Output *GetActiveOutput() const;

  void Clear();

  size_t size() const { return deque_.count(); }

  Output *FindByID(uint32_t id) const;

 private:

  OutputPrivateDeque deque_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_OUTPUT_MANAGER_HPP_
