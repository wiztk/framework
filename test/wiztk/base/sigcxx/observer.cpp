//
// Created by zhanggyb on 17-9-7.
//

#include "observer.hpp"

#include "iostream"

Observer::Observer() {
  std::cout << __func__ << std::endl;
}

Observer::~Observer() {
  std::cout << __func__ << std::endl;
}

void Observer::OnSignal1(int count, wiztk::base::SLOT slot) {
  count1_ = count;
}

void Observer::OnSignal2(int count1, int count2, wiztk::base::SLOT slot) {
  count1_ = count1;
  count2_ = count2;
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