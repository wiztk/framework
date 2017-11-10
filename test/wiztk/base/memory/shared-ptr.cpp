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

#include "shared-ptr.hpp"

#include "wiztk/base/memory.hpp"

using namespace wiztk;
using namespace wiztk::base;

class RefObject : public SPCountedBase {

 public:

  RefObject() {
    std::cout << __func__ << std::endl;
  }

  virtual ~RefObject() {
    std::cout << __func__ << std::endl;
  }

};

SharedPtrTest::SharedPtrTest() {

}

SharedPtrTest::~SharedPtrTest() {

}

TEST_F(SharedPtrTest, construct_1) {
  SharedPtr<RefObject> ptr(new RefObject);
  ASSERT_TRUE(ptr->use_count() == 1);
}

TEST_F(SharedPtrTest, construct_2) {
  SharedPtr<RefObject> ptr1(new RefObject);
  SharedPtr<RefObject> ptr2(ptr1);

  ASSERT_TRUE((ptr1->use_count() == 2) && (ptr2->use_count() == 2));
}

TEST_F(SharedPtrTest, construct_3) {
  SharedPtr<RefObject> ptr1(new RefObject);
  SharedPtr<RefObject> ptr2(std::move(ptr1));

  ASSERT_TRUE((!ptr1) && (ptr2->use_count() == 1));
}

TEST_F(SharedPtrTest, copy_assignment_1) {
  SharedPtr<RefObject> ptr1(new RefObject);

  SharedPtr<RefObject> ptr2 = ptr1;

  ASSERT_TRUE((ptr1->use_count() == 2) && (ptr2->use_count() == 2));
}

TEST_F(SharedPtrTest, move_assignment_1) {
  SharedPtr<RefObject> ptr1(new RefObject);

  SharedPtr<RefObject> ptr2 = std::move(ptr1);

  ASSERT_TRUE((!ptr1) && (ptr2->use_count() == 1));
}

TEST_F(SharedPtrTest, reset_1) {
  SharedPtr<RefObject> ptr;

  ptr.Reset(new RefObject);
  ptr.Reset(new RefObject);

  ASSERT_TRUE(ptr->use_count() == 1);
}

TEST_F(SharedPtrTest, reset_2) {
  SharedPtr<RefObject> ptr;

  ptr.Reset(new RefObject);
  ptr.Reset(nullptr);

  ASSERT_TRUE(!ptr);
}

TEST_F(SharedPtrTest, swap_1) {
  SharedPtr<RefObject> shared_ptr1(new RefObject);

  SharedPtr<RefObject> shared_ptr2(new RefObject);

  SharedPtr<RefObject> shared_ptr3(shared_ptr1);

  Swap(shared_ptr1, shared_ptr2);

  ASSERT_TRUE(shared_ptr1->use_count() == 1 && shared_ptr2->use_count() == 2);
}

TEST_F(SharedPtrTest, swap_2) {
  SharedPtr<RefObject> shared_ptr1(new RefObject);

  SharedPtr<RefObject> shared_ptr2(new RefObject);

  SharedPtr<RefObject> shared_ptr3(shared_ptr1);

  shared_ptr1.Swap(shared_ptr2);

  ASSERT_TRUE(shared_ptr1->use_count() == 1 && shared_ptr2->use_count() == 2);
}

TEST_F(SharedPtrTest, unique_1) {
  SharedPtr<RefObject> shared_ptr1;
  SharedPtr<RefObject> shared_ptr2(new RefObject);

  ASSERT_TRUE(!shared_ptr1.IsUnique());
  shared_ptr1 = shared_ptr2;
  ASSERT_TRUE(!shared_ptr1.IsUnique());

  shared_ptr2 = nullptr;

  std::cout << shared_ptr1.use_count() << std::endl;
  std::cout << shared_ptr2.use_count() << std::endl;

  ASSERT_TRUE(shared_ptr1.IsUnique());
}
