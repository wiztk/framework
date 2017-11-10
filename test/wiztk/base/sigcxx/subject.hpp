//
// Created by zhanggyb on 17-9-7.
//

#ifndef wiztk_TEST_base_SIGCXX_SUBJECT_HPP_
#define wiztk_TEST_base_SIGCXX_SUBJECT_HPP_

#include "wiztk/base/sigcxx.hpp"

class Subject : public wiztk::base::Trackable {

 public:

  template<typename ... ParamTypes>
  using SignalRef = typename wiztk::base::SignalRef<ParamTypes...>;

  template<typename ... ParamTypes>
  using Signal = typename wiztk::base::Signal<ParamTypes...>;

  Subject() = default;

  ~Subject() final = default;

  SignalRef<int> signal1() { return signal1_; }

  SignalRef<int, int> signal2() { return signal2_; };

  void Test(int count) { signal1_.Emit(count); }

  void Test(int count1, int count2) { signal2_.Emit(count1, count2); }

 private:

  Signal<int> signal1_;
  Signal<int, int> signal2_;

};

#endif // wiztk_TEST_base_SIGCXX_SUBJECT_HPP_
