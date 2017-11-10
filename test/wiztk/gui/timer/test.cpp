//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <wiztk/gui/application.hpp>
#include <wiztk/gui/window.hpp>

#include <wiztk/gui/abstract-view.hpp>
#include <wiztk/gui/timer.hpp>
#include <wiztk/gui/mouse-event.hpp>
#include <wiztk/gui/key-event.hpp>

#include <wiztk/graphic/paint.hpp>
#include <wiztk/graphic/canvas.hpp>

#include <wiztk/gui/context.hpp>

using namespace wiztk;
using namespace wiztk::gui;
using namespace wiztk::base;

class TimerWatcher : public Trackable {
 public:

  TimerWatcher(Timer *timer)
      : timer_(timer), count_(0) {}

  virtual ~TimerWatcher() {}

  void OnTimeout(__SLOT__) {
    fprintf(stdout, "%s\n", __PRETTY_FUNCTION__);
    count_++;
    if (count_ == 10) {
      timer_->Stop();
      Application::instance()->Exit();
    }
  }

 private:

  Timer *timer_;
  int count_;

};

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

/*
 *
 */
TEST_F(Test, timer_1) {
  int argc = 1;
  char argv1[] = "resize_1";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  Timer t;
  TimerWatcher watcher(&t);

  t.timeout().Connect(&watcher, &TimerWatcher::OnTimeout);
  t.SetInterval(1000000);
  t.Start();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}
