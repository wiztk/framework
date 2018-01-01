//

#include "test-thread.hpp"

#include "wiztk/system/threading/thread.hpp"

#include <iostream>

using namespace wiztk;
using namespace wiztk::system;

class MyThread : public threading::Thread {

 public:

  using threading::Thread::Thread;

 protected:

  void Run() final {
    for (int i = 0; i < 10; ++i) {
      sleep(1);
      std::cout << __func__ << std::endl;
    }
  }

};

TEST_F(TestThread, construct_1) {
  MyThread t;

  t.Start();
  t.Join();

  ASSERT_TRUE(true);
}
