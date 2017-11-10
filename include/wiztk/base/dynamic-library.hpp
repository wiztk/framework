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

#ifndef WIZTK_CORE_DYNAMIC_LIBRARY_HPP_
#define WIZTK_CORE_DYNAMIC_LIBRARY_HPP_

#include <dlfcn.h>

namespace wiztk {
namespace base {

/**
 * @ingroup base
 * @brief dlopen wrapper
 */
class DynamicLibrary {

 public:

  DynamicLibrary()
      : handle_(nullptr) {}

  ~DynamicLibrary();

  void Open(const char *filename, int flags);

  void Close();

  void *GetSymbol(const char *symbol);

  bool IsValid() const { return nullptr != handle_; }

 private:

  void *handle_;

};

} // namespace base
} // namespace wiztk

#endif // WIZTK_CORE_DYNAMIC_LIBRARY_HPP_
