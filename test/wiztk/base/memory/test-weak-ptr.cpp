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

#include "test-weak-ptr.hpp"

#include "wiztk/base/macros.hpp"
#include "wiztk/base/memory/weak-ptr.hpp"

using namespace wiztk;
using namespace wiztk::base;

class MyRefCounted : public RefCountedBase<> {

 public:

  MyRefCounted() {
    std::cout << __func__ << std::endl;
  }

  ~MyRefCounted() final {
    std::cout << __func__ << std::endl;
  }

};

class A;
class B;

class A : public RefCountedBase<> {
 public:

  A() {
    _DEBUG("%s\n", __func__);
  }

  virtual ~A() {
    _DEBUG("%s\n", __func__);
  }

  SharedPtr<B> b_;
};

class B : public RefCountedBase<> {
 public:

  B() {
    _DEBUG("%s\n", __func__);
  }

  virtual ~B() {
    _DEBUG("%s\n", __func__);
  }

  WeakPtr<A> a_;
};

TEST_F(TestWeakPtr, construct_1) {

  SharedPtr<MyRefCounted> shared_ptr(new MyRefCounted);
  WeakPtr<MyRefCounted> weak_ptr(shared_ptr);

  ASSERT_TRUE(shared_ptr.use_count() == 1 && shared_ptr.weak_count() == 1);
}

TEST_F(TestWeakPtr, construct_2) {

  SharedPtr<MyRefCounted> shared_ptr(new MyRefCounted);
  WeakPtr<MyRefCounted> weak_ptr1(shared_ptr);
  WeakPtr<MyRefCounted> weak_ptr2(weak_ptr1);

  ASSERT_TRUE(shared_ptr.use_count() == 1 && shared_ptr.weak_count() == 2);
}

TEST_F(TestWeakPtr, construct_3) {

  SharedPtr<MyRefCounted> shared_ptr(new MyRefCounted);
  WeakPtr<MyRefCounted> weak_ptr1(shared_ptr);

  shared_ptr.Reset(nullptr);

  ASSERT_TRUE((!shared_ptr) && (weak_ptr1.use_count() == 0));
}

TEST_F(TestWeakPtr, assignment_1) {
  SharedPtr<MyRefCounted> shared_ptr(new MyRefCounted);
  WeakPtr<MyRefCounted> weak_ptr;
  weak_ptr = shared_ptr;

  ASSERT_TRUE(shared_ptr.use_count() == 1 && shared_ptr.weak_count() == 1);
}

TEST_F(TestWeakPtr, assignment_2) {
  SharedPtr<MyRefCounted> shared_ptr(new MyRefCounted);
  WeakPtr<MyRefCounted> weak_ptr1;
  weak_ptr1 = shared_ptr;

  WeakPtr<MyRefCounted> weak_ptr2;
  weak_ptr2 = weak_ptr1;

  ASSERT_TRUE(shared_ptr.use_count() == 1 && shared_ptr.weak_count() == 2);
}

TEST_F(TestWeakPtr, reset_1) {
  SharedPtr<MyRefCounted> shared_ptr(new MyRefCounted);
  WeakPtr<MyRefCounted> weak_ptr;

  weak_ptr.Reset();

  ASSERT_TRUE(weak_ptr.use_count() == 0 && shared_ptr.use_count() == 1);
}

TEST_F(TestWeakPtr, cross_reference_1) {
  SharedPtr<A> a(new A);
  SharedPtr<B> b(new B);

  a->b_ = b;
  b->a_ = a;

  _DEBUG("a.use_count(): %ld\n", a.use_count());
  _DEBUG("b.use_count(): %ld\n", b.use_count());

  ASSERT_TRUE(a.use_count() == 1);
  ASSERT_TRUE(b.use_count() == 2);

  ASSERT_TRUE(a->b_.use_count() == 2 && a->b_.weak_count() == 0);
  ASSERT_TRUE(b->a_.use_count() == 1 && b->a_.weak_count() == 1);

//  a.reset();
//  b.reset();
}

TEST_F(TestWeakPtr, bool_1) {
  SharedPtr<MyRefCounted> shared_ptr1(new MyRefCounted);
  WeakPtr<MyRefCounted> weak_ptr1(shared_ptr1);
//  ASSERT_TRUE(weak_ptr1);

  weak_ptr1.Reset();
  ASSERT_TRUE(!weak_ptr1);
}

TEST_F(TestWeakPtr, swap_1) {
  SharedPtr<MyRefCounted> shared_ptr1(new MyRefCounted);
  WeakPtr<MyRefCounted> weak_ptr1(shared_ptr1);
  ASSERT_TRUE(weak_ptr1.use_count() == 1 && weak_ptr1.weak_count() == 1);

  SharedPtr<MyRefCounted> shared_ptr2(new MyRefCounted);
  ASSERT_TRUE(shared_ptr2.use_count() == 1 && shared_ptr2.weak_count() == 0);

  SharedPtr<MyRefCounted> shared_ptr3(shared_ptr2);
  WeakPtr<MyRefCounted> weak_ptr2(shared_ptr2);
  ASSERT_TRUE(weak_ptr2.use_count() == 2 && weak_ptr2.weak_count() == 1);

  weak_ptr1.Swap(weak_ptr2);
  ASSERT_TRUE(weak_ptr1.use_count() == 2 &&
      weak_ptr1.weak_count() == 1 &&
      weak_ptr2.use_count() == 1 &&
      weak_ptr2.weak_count() == 1);
}
