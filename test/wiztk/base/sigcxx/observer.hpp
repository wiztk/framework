#ifndef WIZTK_TEST_BASE_SIGCXX_OBSERVER_HPP_
#define WIZTK_TEST_BASE_SIGCXX_OBSERVER_HPP_

#include <vector>

#include "wiztk/base/sigcxx.hpp"

class Observer : public wiztk::base::Trackable {

 public:

  Observer();

  ~Observer() final;

  void OnCount1(int, __SLOT__);

  void OnCount2(int, int, __SLOT__);

  void OnUnbindSlot(int, __SLOT__);

  void OnUnbindAllSignals(int, __SLOT__);

  /**
   * @brief Delete this object when called
   * @param slot
   */
  void OnDeleteThis(int, __SLOT__);

  int count1() const { return count1_; }

  const std::vector<int> &count2() const { return count2_; }

 private:

  int count1_ = 0;

  std::vector<int> count2_ = {0, 0};

};

#endif // WIZTK_TEST_BASE_SIGCXX_OBSERVER_HPP_
