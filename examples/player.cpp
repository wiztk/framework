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
//#include <wiztk/av/format.hpp>

#include <iostream>

#include "wiztk/base/memory.hpp"

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

class A;
class B;

class A : public SPCountedBase {
 public:

  A() {
    _DEBUG("%s\n", __func__);
  }

  virtual ~A() {
    _DEBUG("%s\n", __func__);
  }

  SharedPtr<B> b_;
};

class B : public SPCountedBase {
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

  SharedPtr<B> b(new B);
  SharedPtr<A> a(new A);

  a->b_ = b;
  b->a_ = a;

  _DEBUG("a use_count: %ld, weak_count: %ld\n", a.use_count(), a.weak_count());
  _DEBUG("b use_count: %ld, weak_count: %ld\n", b.use_count(), b.weak_count());

  return 0;
}
