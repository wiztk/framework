
#include "test-address-info.hpp"

#include "wiztk/net/address-info.hpp"

using namespace wiztk;
using namespace wiztk::net;

TEST_F(TestAddressInfo, get_1) {
  std::unique_ptr<AddressInfoList> list = AddressInfo::GetAll("www.baidu.com", "http");

  ASSERT_TRUE(list->size() != 0);
}
