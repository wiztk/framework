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

#include "test-shared-ptr.hpp"

#include "wiztk/base/memory/ref-counted-base.hpp"
#include "wiztk/base/memory/shared-ptr.hpp"

using namespace wiztk;
using namespace wiztk::base;
using namespace wiztk::base::memory;

class MyRefCounted : public RefCountedBase<> {

 public:

  MyRefCounted() {
    std::cout << __func__ << std::endl;
  }

  ~MyRefCounted() final {
    std::cout << __func__ << std::endl;
  }

};

TEST_F(TestSharedPtr, construct_1) {
  auto *obj = new MyRefCounted;
  SharedPtr<MyRefCounted> ptr(obj);

  ASSERT_TRUE(
      ptr.use_count() == 1 &&
          ptr.weak_count() == 0
  );
}

TEST_F(TestSharedPtr, construct_2) {
  auto *obj = new MyRefCounted;
  SharedPtr<MyRefCounted> ptr1(obj);
  SharedPtr<MyRefCounted> ptr2(ptr1);

  ASSERT_TRUE(
      ptr1.use_count() == 2 &&
          ptr1.weak_count() == 0
  );
}

TEST_F(TestSharedPtr, construct_3) {
  auto *obj = new MyRefCounted;
  SharedPtr<MyRefCounted> ptr1(obj);
  SharedPtr<MyRefCounted> ptr2(std::move(ptr1));

  ASSERT_TRUE(
      (!ptr1) &&
          ptr2.use_count() == 1
  );
}

TEST_F(TestSharedPtr, copy_assignment_1) {
  auto *obj = new MyRefCounted;
  SharedPtr<MyRefCounted> ptr1(obj);

  SharedPtr<MyRefCounted> ptr2 = ptr1;

  ASSERT_TRUE(
      ptr1.use_count() == 2 &&
          ptr2.use_count() == 2 &&
          ptr1.weak_count() == 0 &&
          ptr2.weak_count() == 0
  );
}

TEST_F(TestSharedPtr, move_assignment_1) {
  auto *obj = new MyRefCounted;

  SharedPtr<MyRefCounted> ptr1(obj);

  SharedPtr<MyRefCounted> ptr2 = std::move(ptr1);

  ASSERT_TRUE(
      (!ptr1) &&
          ptr1.use_count() == 0 &&
          (ptr2.use_count() == 1) &&
          ptr2.weak_count() == 0
  );
}

TEST_F(TestSharedPtr, reset_1) {
  SharedPtr<MyRefCounted> ptr;

  ptr.Reset(new MyRefCounted);
  ptr.Reset(new MyRefCounted);

  ASSERT_TRUE(ptr.use_count() == 1);
}

TEST_F(TestSharedPtr, reset_2) {
  SharedPtr<MyRefCounted> ptr;

  ptr.Reset(new MyRefCounted);
  ptr.Reset(nullptr);

  ASSERT_TRUE(!ptr);
}

TEST_F(TestSharedPtr, swap_2) {
  SharedPtr<MyRefCounted> shared_ptr1(new MyRefCounted);

  SharedPtr<MyRefCounted> shared_ptr2(new MyRefCounted);

  SharedPtr<MyRefCounted> shared_ptr3(shared_ptr1);

  shared_ptr1.Swap(shared_ptr2);

  ASSERT_TRUE(shared_ptr1.use_count() == 1 && shared_ptr2.use_count() == 2);
}

TEST_F(TestSharedPtr, unique_1) {
  SharedPtr<MyRefCounted> shared_ptr1;
  SharedPtr<MyRefCounted> shared_ptr2(new MyRefCounted);

  ASSERT_TRUE(!shared_ptr1.IsUnique());
  shared_ptr1 = shared_ptr2;
  ASSERT_TRUE(!shared_ptr1.IsUnique());

  shared_ptr2 = nullptr;

  std::cout << shared_ptr1.use_count() << std::endl;
  std::cout << shared_ptr2.use_count() << std::endl;

  ASSERT_TRUE(shared_ptr1.IsUnique());
}

TEST_F(TestSharedPtr, move_1) {
  SharedPtr<MyRefCounted> shared_ptr1(new MyRefCounted);
  SharedPtr<MyRefCounted> shared_ptr2 = std::move(shared_ptr1);

  ASSERT_TRUE((!shared_ptr1) && (shared_ptr2));
}