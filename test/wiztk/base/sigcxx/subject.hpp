#ifndef WIZTK_TEST_BASE_SIGCXX_SUBJECT_HPP_
#define WIZTK_TEST_BASE_SIGCXX_SUBJECT_HPP_

#include "wiztk/base/sigcxx.hpp"

class Subject : public wiztk::base::Trackable {

 public:

  template<typename ... Args>
  using SignalRef = typename wiztk::base::SignalRef<Args...>;

  template<typename ... Args>
  using Signal = typename wiztk::base::Signal<Args...>;

  Subject() = default;

  ~Subject() final = default;

  SignalRef<int> count1() { return count1_; }

  SignalRef<int, int> count2() { return count2_; };

  void DoCount1(int count) { count1_.Emit(count); }

  void DoCount2(int count1, int count2) { count2_.Emit(count1, count2); }

 private:

  Signal<int> count1_;
  Signal<int, int> count2_;

};

#endif // WIZTK_TEST_BASE_SIGCXX_SUBJECT_HPP_
