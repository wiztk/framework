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

#include "wiztk/base/string.hpp"

#include <unicode/utf.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

namespace wiztk {
namespace base {

std::ostream &operator<<(std::ostream &out, const String16 &utf16) {
  // TODO: This create and copy the string array from String16 to icu::UnicodeString, find a better way for performance.
  return out << icu::UnicodeString(reinterpret_cast<const UChar *>(utf16.data()));
}

std::ostream &operator<<(std::ostream &out, const String32 &utf32) {
  // TODO: This create and copy the string array from String16 to icu::UnicodeString, find a better way for performance.
  return out << icu::UnicodeString::fromUTF32(reinterpret_cast<const UChar32 *>(utf32.data()),
                                              static_cast<uint32_t >(utf32.length()));
}

} // namespace base
} // namespace wiztk
