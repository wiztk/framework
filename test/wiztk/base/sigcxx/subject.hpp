//
// Created by zhanggyb on 17-9-7.
//

#ifndef wiztk_TEST_base_SIGCXX_SUBJECT_HPP_
#define wiztk_TEST_base_SIGCXX_SUBJECT_HPP_

#include "wiztk/base/sigcxx.hpp"

class Subject : public wiztk::base::Trackable {

 public:

  template<typename ... ParamTypes>
  using SignalRefT = typename wiztk::base::SignalRefT<ParamTypes...>;

  template<typename ... ParamTypes>
  using SignalT = typename wiztk::base::SignalT<ParamTypes...>;

  Subject() = default;

  ~Subject() final = default;

  SignalRefT<int> signal1() { return signal1_; }

  SignalRefT<int, int> signal2() { return signal2_; };

  void Test(int count) { signal1_.Emit(count); }

  void Test(int count1, int count2) { signal2_.Emit(count1, count2); }

 private:

  SignalT<int> signal1_;
  SignalT<int, int> signal2_;

};

#endif // wiztk_TEST_base_SIGCXX_SUBJECT_HPP_
