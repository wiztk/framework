//

#include "test-clock.hpp"

#include "wiztk/system/time/realtime-clock.hpp"

#include <iostream>

using namespace wiztk;
using namespace wiztk::system;

TEST_F(TestClock, construct_1) {
  time::RealtimeClock clock(2017, 11, 28, 10, 30);
  std::string str = clock.ToString();

  std::cout << str << std::endl;

  ASSERT_TRUE(true);
}

TEST_F(TestClock, now_1) {
  time::RealtimeClock time = time::RealtimeClock::Now();
  std::string str = time.ToString();

  std::cout << str << std::endl;

  ASSERT_TRUE(true);
}
