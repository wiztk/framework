
#include "test-ip-address.hpp"

#include "wiztk/net/ip-address.hpp"

using std::cout;
using std::endl;

using namespace wiztk;
using namespace wiztk::net;

TEST_F(TestIPAddress, get_by_name_1) {
  std::unique_ptr<IPAddressList> list = IPAddress::GetByName("www.baidu.com");

  if (list->size() > 0) {
    for (size_t i = 0; i < list->size(); ++i) {
      cout << "address: " << list->GetAt(i)->ToString() << endl;
    }
  }

  ASSERT_TRUE(list->size() > 0);
}