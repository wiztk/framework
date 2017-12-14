
#include "test-ip-address.hpp"

#include "wiztk/net/ip-address.hpp"
#include "wiztk/net/address-info.hpp"

using std::cout;
using std::endl;

using namespace wiztk;
using namespace wiztk::net;

TEST_F(TestIPAddress, get_by_name_1) {
  std::unique_ptr<IPAddressList> list = IPAddress::GetByName("www.baidu.com");

  if (list->size() > 0) {
    for (size_t i = 0; i < list->size(); ++i) {
      cout << "address: " << (*list)[i]->ToString() << endl;
    }
  }

  ASSERT_TRUE(list->size() > 0);
}

TEST_F(TestIPAddress, get_by_host_and_service_1) {
  std::unique_ptr<IPAddressList> list = IPAddress::GetByHostAndService("www.baidu.com");

  if (list->size() > 0) {
    for (size_t i = 0; i < list->size(); ++i) {
      cout << "address: " << (*list)[i]->ToString() << endl;
    }
  }

  ASSERT_TRUE(list->size() > 0);
}

TEST_F(TestIPAddress, construct_1) {
  std::unique_ptr<AddressInfoList> list = AddressInfo::GetAll("www.baidu.com");

  if (list->size()) {
    IPAddress address(*(*list)[0]);
    cout << "address: " << address.ToString() << endl;
  }

  ASSERT_TRUE(list->size() > 0);
}

TEST_F(TestIPAddress, get_host_and_service_1) {
  std::unique_ptr<AddressInfoList> list = AddressInfo::GetAll("www.baidu.com", "http");

  IPAddress address(*(*list)[0]);

  IPAddress::NameInfo names = address.ToHostAndService();
  cout << "host: " << names.first << endl;
  cout << "service: " << names.second << endl;

  ASSERT_TRUE(list->size() > 0);
}
