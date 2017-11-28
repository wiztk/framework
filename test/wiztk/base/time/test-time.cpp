//

#include "test-time.hpp"

#include "wiztk/base/time.hpp"

#include <iostream>

using namespace wiztk;
using namespace wiztk::base;

TEST_F(TestTime, construct_1) {
  using base::Time;

  Time time(2017, 11, 28, 10, 30);
  String str = time.ToString();

  std::cout << str << std::endl;

  ASSERT_TRUE(true);
}

TEST_F(TestTime, now_1) {
  using base::Time;

  Time time = Time::Now();
  String str = time.ToString();

  std::cout << str << std::endl;

  ASSERT_TRUE(true);
}
