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

#include <wiztk/gui/application.hpp>

#include <iostream>

#include "wiztk/base/memory/shared-ptr.hpp"
#include "wiztk/base/memory/weak-ptr.hpp"

using namespace wiztk::base;

class MyRefCounted : public RefCountedBase<> {

 public:

  MyRefCounted() {
    std::cout << __func__ << std::endl;
  }

  virtual ~MyRefCounted() {
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

int main(int argc, char *argv[]) {

//  SharedPtrT<RefCounted> shared_ptr1(new RefCounted);
//  WeakPtrT<RefCounted> weak_ptr1(shared_ptr1);
//  _ASSERT(weak_ptr1.use_count() == 1 && weak_ptr1.weak_count() == 1);
//
//  SharedPtrT<RefCounted> shared_ptr2(new RefCounted);
//  SharedPtrT<RefCounted> shared_ptr3(shared_ptr2);
//  WeakPtrT<RefCounted> weak_ptr2(shared_ptr2);
//  _ASSERT(weak_ptr2.use_count() == 2 && weak_ptr2.weak_count() == 1);

//  Swap(weak_ptr1, weak_ptr2);
//  _ASSERT(weak_ptr1.use_count() == 2 && weak_ptr1.weak_count() == 1 && weak_ptr2.use_count() == 1
//              && weak_ptr2.weak_count() == 1);

  return 0;
}
