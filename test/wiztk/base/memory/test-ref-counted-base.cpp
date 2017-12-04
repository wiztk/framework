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

#include "test-ref-counted-base.hpp"

#include "wiztk/base/memory/atomic-ref-count.hpp"
#include "wiztk/base/memory/ref-counted-thread-safe-base.hpp"

using namespace wiztk;
using namespace wiztk::base;

/**
 * @brief Make sure have the same memory size
 */
//TEST_F(TestRefCountedBase, count_1) {
//  RefCountedBase obj;
//  ASSERT_TRUE(obj.use_count() == 0 && obj.weak_count() == 0);
//}
