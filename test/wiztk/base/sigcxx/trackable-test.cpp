#include "wiztk/base/sigcxx.hpp"

#include "trackable-test.hpp"

#include "observer.hpp"

using namespace wiztk;
using namespace wiztk::base;

TEST_F(TrackableTest, slot_1) {
  Signal<int> signal;
  Observer observer;

  signal.Connect(&observer, &Observer::OnCount1);

  signal(1);  // Emit signal1: 1

  ASSERT_TRUE(observer.count1() == 1);
}

TEST_F(TrackableTest, unbind_signal_1) {
  Signal<int> signal;

  Observer observer1;
  Observer observer2;
  Observer observer3;

  signal.Connect(&observer1, &Observer::OnCount1);
  signal.Connect(&observer2, &Observer::OnUnbindSlot);
  signal.Connect(&observer3, &Observer::OnCount1);

  signal(1);  // Emit signal1: 1

  ASSERT_TRUE(signal.CountConnections() == 2);
}

TEST_F(TrackableTest, unbind_all_signals_1) {
  Signal<int> signal;
  Observer observer1;

  signal.Connect(&observer1, &Observer::OnCount1);
  signal.Connect(&observer1, &Observer::OnUnbindAllSignals);
  signal.Connect(&observer1, &Observer::OnCount1);

  signal(1);  // Emit signal1: 1

  ASSERT_TRUE(signal.CountConnections() == 0);
}

TEST_F(TrackableTest, delete_this_1) {
  Signal<int> signal;

  auto *observer1 = new Observer;
  auto *observer2 = new Observer;
  auto *observer3 = new Observer;

  signal.Connect(observer1, &Observer::OnCount1);
  signal.Connect(observer2, &Observer::OnDeleteThis);
  signal.Connect(observer3, &Observer::OnCount1);

  signal(1);  // Emit signal1: 1

  ASSERT_TRUE(signal.CountConnections() == 2);

  delete observer1;
  delete observer3;
}
