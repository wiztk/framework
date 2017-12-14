
#include "test-address-info.hpp"

#include "wiztk/net/address-info.hpp"
#include "wiztk/net/ip-address.hpp"

using namespace wiztk;
using namespace wiztk::net;

using std::cout;
using std::endl;

TEST_F(TestAddressInfo, getall_1) {
  std::unique_ptr<AddressInfoList> list = AddressInfo::GetAll("www.baidu.com", "http");

  for (size_t i = 0; i < list->size(); ++i) {
    IPAddress ip(*(*list)[i]);
    cout << "address: " << ip.ToString() << endl;
  }

  ASSERT_TRUE(list->size() != 0);
}

TEST_F(TestAddressInfo, getall_2) {
  std::unique_ptr<AddressInfoList> list = AddressInfo::GetAll("localhost");
  ASSERT_TRUE(list->size() != 0);
}
