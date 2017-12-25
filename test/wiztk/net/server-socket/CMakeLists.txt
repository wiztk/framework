file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(net-server-socket ${sources} ${headers})
target_link_libraries(net-server-socket ${GTEST_LIBRARIES} wiztk)
