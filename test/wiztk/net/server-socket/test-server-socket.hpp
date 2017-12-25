//

#ifndef WIZTK_TEST_NET_SERVER_SOCKET_HPP_
#define WIZTK_TEST_NET_SERVER_SOCKET_HPP_

#include <gtest/gtest.h>

class TestServerSocket : public testing::Test {

 public:

  TestServerSocket() = default;

  ~TestServerSocket() override = default;

 protected:

  void SetUp() final {}

  void TearDown() final {}

};

#endif // WIZTK_TEST_NET_SERVER_SOCKET_HPP_
