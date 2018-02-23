//

#include "test-logger.hpp"

#include "wiztk/system/logging/logger.hpp"
#include "wiztk/system/logging/type.hpp"

using namespace wiztk;
using namespace wiztk::system;

TEST_F(TestLogger, log_1) {
  logging::Logger::LogToSystem(logging::kNotice, "Hello World, this is PID %lu!", (unsigned long) getpid());
  ASSERT_TRUE(true);
}
