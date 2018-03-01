file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(net-socket ${sources} ${headers})
target_link_libraries(net-socket ${GTEST_LIBRARIES} wiztk-net)