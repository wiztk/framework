//

#ifndef WIZTK_TEST_NET_SOCKET_HPP_
#define WIZTK_TEST_NET_SOCKET_HPP_

#include <gtest/gtest.h>

class TestSocket : public testing::Test {

 public:

  TestSocket() = default;

  ~TestSocket() override = default;

 protected:

  void SetUp() final {}

  void TearDown() final {}

};

#endif // WIZTK_TEST_NET_SOCKET_HPP_
