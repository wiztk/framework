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

#ifndef WIZTK_OUTPUT_MANAGER_HPP
#define WIZTK_OUTPUT_MANAGER_HPP

#include "wiztk/base/counted-deque.hpp"

#include "wiztk/gui/output.hpp"

namespace wiztk {
namespace gui {

class OutputManager {

 public:

  OutputManager() = default;

  ~OutputManager() = default;

  void Insert(Output *output, int index) {
    deque_.insert(output, index);
  }

  void Clear() {
    deque_.clear([](base::BinodeBase *obj) { delete obj; });
  }

  size_t size() const { return deque_.count(); }

  Output *at(int index) const {
    return deque_.at(index);
  }

 private:

  base::CountedDeque<Output> deque_;

};

}
}

#endif //WIZTK_OUTPUT_MANAGER_HPP
