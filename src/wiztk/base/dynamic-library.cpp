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

#include "wiztk/base/dynamic-library.hpp"

#include <iostream>

namespace wiztk {
namespace base {

using std::cerr;
using std::endl;

DynamicLibrary::~DynamicLibrary() {
  if (handle_) dlclose(handle_);
}

void DynamicLibrary::Open(const char *filename, int flags) {
  Close();

  handle_ = dlopen(filename, flags);
  if (nullptr == handle_) {
    cerr << "Error! Cannot load library: " << dlerror() << endl;
  }
}

void DynamicLibrary::Close() {
  if (handle_) {
    dlclose(handle_);
    handle_ = nullptr;
  }
}

void *DynamicLibrary::GetSymbol(const char *symbol) {
  void* addr = dlsym(handle_, symbol);
  const char* error = dlerror();
  if (error) {
    cerr << "Error! Cannot load symbol: " << error << endl;
  }

  return addr;
}

} // namespace base
} // namespace wiztk
