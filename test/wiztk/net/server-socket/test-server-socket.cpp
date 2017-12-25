
#include "test-server-socket.hpp"

#include "wiztk/net/ip-address.hpp"
#include "wiztk/net/server-socket.hpp"
#include "wiztk/net/io-buffer.hpp"

using namespace wiztk;
using namespace wiztk::net;

TEST_F(TestServerSocket, connect_1) {
  std::unique_ptr<IPAddressList> list = IPAddress::GetByHostAndService("127.0.0.1");

  ServerSocket server = ServerSocket(kAddressFamilyINET, kSocketStream);
  std::cout << list->at(0)->ToString() << std::endl;
  server.Bind(*list->at(0));

  server.Listen(50);

  size_t buffer_size = 512;

  IOBuffer buf(buffer_size);
  size_t size = 0;
  std::string echo = "echo";
  while (true) {

    server.Accept();

    size = server.Receive(buf, buffer_size);

    if (size == 0) {
      std::cout << buf.data() << std::endl;
      memcpy(buf.data(), echo.data(), echo.size());
      size = server.Send(buf, buffer_size);
      break;
    }
  }

  server.Close();

  ASSERT_TRUE(true);
}
