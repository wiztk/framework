//
// Created by zhanggyb on 17-8-25.
//

#ifndef SKLAND_LOCKGUARDTEST_HPP
#define SKLAND_LOCKGUARDTEST_HPP

#include <gtest/gtest.h>

class LockGuardTest : public testing::Test {

 public:

  LockGuardTest();

  virtual ~LockGuardTest();

 protected:

  virtual void SetUp() {}

  virtual void TearDown() {}

};

#endif //SKLAND_LOCKGUARDTEST_HPP
