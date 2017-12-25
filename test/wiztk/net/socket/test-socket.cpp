
#include "test-socket.hpp"

#include "wiztk/net/socket.hpp"
#include "wiztk/net/ip-address.hpp"
#include "wiztk/net/address-info.hpp"
#include "wiztk/net/io-buffer.hpp"

using namespace wiztk;
using namespace wiztk::net;

//TEST_F(TestSocket, connect_1) {
//  std::unique_ptr<IPAddressList> list = IPAddress::GetByHostAndService("www.baidu.com", "http");
//
//  Socket socket(kAddressFamilyINET, kSocketStream);
//  std::cout << list->at(0)->ToString() << std::endl;
//
//  socket.Connect(*list->at(0));
//
//  IOBuffer buf(512);
//  std::string hello = "Hello There!";
//  memcpy(buf.data(), hello.data(), hello.size());
//
//  size_t size = socket.Send(buf, 512);
//  if (size > 0) {
//    size = socket.Receive(buf, 512);
//
//    std::cout << "buf received: " << size << std::endl;
//  }
//
//  ASSERT_TRUE(true);
//}

TEST_F(TestSocket, get_address_family_1) {
  std::unique_ptr<IPAddressList> list = IPAddress::GetByHostAndService("www.baidu.com", "http");

  ASSERT_TRUE(list->size() > 0);

  IPAddress *addr = list->at(0);

  Socket socket(kAddressFamilyINET, kSocketStream);
  AddressFamily af = addr->GetAddressFamily();

  ASSERT_TRUE(af == kAddressFamilyINET);
}
