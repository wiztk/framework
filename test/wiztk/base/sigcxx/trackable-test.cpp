//
// Created by zhanggyb on 16-9-19.
//

#include "trackable-test.hpp"

#include <wiztk/base/sigcxx.hpp>

#include "subject.hpp"
#include "observer.hpp"

using namespace wiztk;
using namespace wiztk::base;

TEST_F(TrackableTest, slot_1) {
  Subject subject;
  Observer observer;

  subject.signal1().Connect(&observer, &Observer::OnSignal1);

  subject.Test(1);  // Emit signal1: 1

  ASSERT_TRUE(observer.count1() == 1);
}

TEST_F(TrackableTest, unbind_signal_1) {
  Subject subject;
  Observer observer1;
  Observer observer2;
  Observer observer3;

  subject.signal1().Connect(&observer1, &Observer::OnSignal1);
  subject.signal1().Connect(&observer2, &Observer::OnUnbindSlot);
  subject.signal1().Connect(&observer3, &Observer::OnSignal1);

  subject.Test(1);  // Emit signal1: 1

  ASSERT_TRUE(subject.signal1().CountConnections() == 2);
}

TEST_F(TrackableTest, unbind_all_signals_1) {
  Subject subject;
  Observer observer1;

  subject.signal1().Connect(&observer1, &Observer::OnSignal1);
  subject.signal1().Connect(&observer1, &Observer::OnUnbindAllSignals);
  subject.signal1().Connect(&observer1, &Observer::OnSignal1);

  subject.Test(1);  // Emit signal1: 1

  ASSERT_TRUE(subject.signal1().CountConnections() == 0);
}

TEST_F(TrackableTest, delete_this_1) {
  Subject subject;
  auto* observer1 = new Observer;
  auto* observer2 = new Observer;
  auto* observer3 = new Observer;

  subject.signal1().Connect(observer1, &Observer::OnSignal1);
  subject.signal1().Connect(observer2, &Observer::OnDeleteThis);
  subject.signal1().Connect(observer3, &Observer::OnSignal1);

  subject.Test(1);  // Emit signal1: 1

  ASSERT_TRUE(subject.signal1().CountConnections() == 2);

  delete observer1;
  delete observer3;
}
