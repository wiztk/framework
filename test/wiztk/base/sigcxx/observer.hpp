//
// Created by zhanggyb on 17-9-7.
//

#ifndef wiztk_TEST_CORE_SIGCXX_OBSERVER_HPP_
#define wiztk_TEST_CORE_SIGCXX_OBSERVER_HPP_

#include "wiztk/base/sigcxx.hpp"

class Observer : public wiztk::base::Trackable {

 public:

  Observer();

  ~Observer() final;

  void OnSignal1(int, __SLOT__);

  void OnSignal2(int, int, __SLOT__);

  void OnUnbindSlot(int, __SLOT__);

  void OnUnbindAllSignals(int, __SLOT__);

  /**
   * @brief Delete this object when called
   * @param slot
   */
  void OnDeleteThis(int, __SLOT__);

  int count1() const { return count1_; }

  int count2() const { return count2_; }

 private:

  int count1_ = 0;

  int count2_ = 0;

};

#endif //wiztk_OBSERVER_HPP
