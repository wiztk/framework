//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <wiztk/base/delegate.hpp>

using namespace wiztk;
using namespace wiztk::base;

class Mockup {

 public:

  Mockup() : count_(0) {}

  virtual ~Mockup() {}

  int Foo(int param) {
    count_ = param;
    return count_;
  }

  int ConstFoo(int param) const {
    return count_;
  }

  virtual int VirtualFoo(int param) const {
    return param + count_;
  }

  int count() const { return count_; }

  static int Add(int a, int b) {
    return a + b;
  }

 private:

  int count_;

};

class MockupSub : public Mockup {

 public:

  MockupSub() : Mockup() {}

  virtual ~MockupSub() {}

  virtual int VirtualFoo(int param) const override {
    return param - count();
  }
};

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, constructor1) {
  Mockup obj;
  DelegateT<int(int)> d(&obj, &Mockup::Foo);
  ASSERT_TRUE(1 == d(1));
}

TEST_F(Test, constructor2) {
  Mockup obj;
  DelegateT<int(int)> d = DelegateT<int(int)>::FromMethod(&obj, &Mockup::Foo);
  ASSERT_TRUE(1 == d(1));
}

TEST_F(Test, constructor3) {
  Mockup obj;
  DelegateT<int(int)> d(&obj, &Mockup::ConstFoo);
  ASSERT_TRUE(0 == d(1));
}

TEST_F(Test, constructor4) {
  Mockup obj;
  DelegateT<int(int)> d = DelegateT<int(int)>::FromMethod(&obj, &Mockup::ConstFoo);
  ASSERT_TRUE(0 == d(1));
}

/*
 * Delegate to virtual
 */
TEST_F(Test, constructor5) {
  Mockup *obj = new MockupSub;
  DelegateT<int(int)> d = DelegateT<int(int)>::FromMethod(obj, &Mockup::VirtualFoo);
  obj->Foo(1);  // count_ == 1

  int result = d(2);

  delete obj;

  ASSERT_TRUE(result == 1);
}

TEST_F(Test, constructor6) {
  DelegateT<int(int, int)> d(Mockup::Add);

  int result = 0;
  result = d(1, 2);

  ASSERT_TRUE(result == 3);
}

/*
 *
 */
TEST_F(Test, compare1) {
  Mockup obj;
  DelegateT<int(int)> d = DelegateT<int(int)>::FromMethod(&obj, &Mockup::Foo);
  // const not equal to non-const
  ASSERT_TRUE(d.Equal(&obj, &Mockup::Foo));
}

TEST_F(Test, compare2) {
  Mockup obj;
  DelegateT<int(int)> d = DelegateT<int(int)>::FromMethod(&obj, &Mockup::Foo);
  // const not equal to non-const
  ASSERT_TRUE(!d.Equal(&obj, &Mockup::ConstFoo));
}

TEST_F(Test, compare3) {
  Mockup obj1;
  DelegateT<int(int)> d1 = DelegateT<int(int)>::FromMethod(&obj1, &Mockup::Foo);
  DelegateT<int(int)> d2 = DelegateT<int(int)>::FromMethod(&obj1, &Mockup::Foo);
  ASSERT_TRUE(d1 == d2);
}

TEST_F(Test, compare4) {
  Mockup obj1;
  DelegateT<int(int)> d1 = DelegateT<int(int)>::FromMethod(&obj1, &Mockup::ConstFoo);
  DelegateT<int(int)> d2 = DelegateT<int(int)>::FromMethod(&obj1, &Mockup::Foo);
  ASSERT_TRUE(d1 != d2);
}

TEST_F(Test, compare5) {
  DelegateT<int(int, int)> d(Mockup::Add);

  ASSERT_TRUE(d.Equal(Mockup::Add));
}

TEST_F(Test, delegate_ref_1) {
  Mockup obj;

  DelegateT<int(int)> d1(&obj, &Mockup::Foo);
  DelegateT<int(int)> d2(&obj, &Mockup::ConstFoo);

  DelegateRefT<int(int)> r1(d1);
  DelegateRefT<int(int)> r2(d2);

  r1.Bind(&obj, &Mockup::Foo);
  r2.Bind(&obj, &Mockup::ConstFoo);

  ASSERT_TRUE(r1 && r2);

  bool result1 = r1.IsAssignedTo(&obj, &Mockup::Foo);
  bool result2 = r2.IsAssignedTo(&obj, &Mockup::ConstFoo);

  ASSERT_TRUE(result1 && result2);

  r1.Reset();
  r2.Reset();

  ASSERT_TRUE((!r1) && (!r2));
}
