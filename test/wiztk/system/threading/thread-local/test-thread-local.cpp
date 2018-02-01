//

#include "test-thread-local.hpp"

#include "wiztk/system/threading/thread.hpp"
#include "wiztk/system/threading/thread-local.hpp"

using namespace wiztk;
using namespace wiztk::system;
using namespace wiztk::system::threading;

class MyThread : public threading::Thread {

 public:

  explicit MyThread(ThreadLocal<int> *local)
      : Thread(), local_(local) {}

  ~MyThread() final = default;

 protected:

  void Run() final {
    local_->Set(&kCount);
    for (int i = 0; i < 10; ++i) {
      sleep(1);
      std::cout << "num: " << kCount << std::endl;
      kCount++;
    }
  }

 private:

  ThreadLocal<int> *local_ = nullptr;

  static int kCount;

};

int MyThread::kCount = 0;

TEST_F(TestThreadLocal, initial_1) {
  ThreadLocal<int> mylocal;

  ASSERT_TRUE(mylocal.Get() == nullptr);
}

TEST_F(TestThreadLocal, construct_1) {
  ThreadLocal<int> mylocal;

  int value = 0;
  mylocal.Set(&value);
  value++;

  MyThread thread(&mylocal);
  thread.Start();
  thread.Join();

  int *d = mylocal.Get();

  ASSERT_TRUE(*d == 1);
}
