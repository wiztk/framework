#include "observer.hpp"

#include "iostream"

Observer::Observer() {
  std::cout << __func__ << std::endl;
}

Observer::~Observer() {
  std::cout << __func__ << std::endl;
}

void Observer::OnCount1(int count, wiztk::base::SLOT slot) {
  count1_ = count;
}

void Observer::OnCount2(int count1, int count2, wiztk::base::SLOT slot) {
  count2_[0] = count1;
  count2_[1] = count2;
}

void Observer::OnUnbindSlot(int, wiztk::base::SLOT slot) {
  UnbindSignal(slot);
}

void Observer::OnUnbindAllSignals(int, wiztk::base::SLOT slot) {
  UnbindAllSignals();
}

void Observer::OnDeleteThis(int, wiztk::base::SLOT slot) {
  delete this;
}
