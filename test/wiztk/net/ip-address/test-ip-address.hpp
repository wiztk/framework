//
//

#ifndef WIZTK_TEST_NET_IP_ADDRESS_HPP_
#define WIZTK_TEST_NET_IP_ADDRESS_HPP_

#include <gtest/gtest.h>

class TestIPAddress : public testing::Test {

 public:

  TestIPAddress() = default;

  ~TestIPAddress() override = default;

 protected:

  void SetUp() final {}

  void TearDown() final {}

};

#endif // WIZTK_TEST_NET_IP_ADDRESS_HPP_
