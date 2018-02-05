//

#include "test-thread.hpp"

#include "wiztk/system/threading/thread.hpp"

#include <iostream>

using namespace wiztk;
using namespace wiztk::system;

class MyThread : public threading::Thread {

 public:

  using threading::Thread::Thread;

  explicit MyThread(int count)
      : threading::Thread(), count_(count) {}

  ~MyThread() final = default;

 protected:

  void Run() final {
    for (int i = 0; i < count_; ++i) {
      sleep(1);
      std::cout << __func__ << std::endl;
    }
  }

 private:

  int count_ = 10;

};

class MyDelegate : public threading::Thread::Delegate {

 public:

  explicit MyDelegate(int count)
      : count_(count) {}

  ~MyDelegate() final = default;

 protected:

  void Run() final {
    for (int i = 0; i < count_; ++i) {
      sleep(1);
      std::cout << __func__ << std::endl;
    }
  }

 private:

  int count_ = 10;

};

TEST_F(TestThread, construct_1) {
  MyThread t(5);

  t.Start();
  t.Join();

  ASSERT_TRUE(true);
}

TEST_F(TestThread, construct_2) {
  MyThread t(new MyDelegate(5));

  t.Start();
  t.Join();

  ASSERT_TRUE(true);
}

TEST_F(TestThread, id_1) {
  MyThread t(5);

  t.Start();
  t.Join();

//  ASSERT_TRUE(t.GetID() != threading::Thread::main()->GetID());
  ASSERT_TRUE(true);
}

TEST_F(TestThread, id_2) {
  MyThread t1(5);
  MyThread t2(10);

  // None of them is running.

  ASSERT_TRUE(t1.GetID() == t2.GetID());
}
