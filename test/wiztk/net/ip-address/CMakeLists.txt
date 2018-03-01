file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(net-ip-address ${sources} ${headers})
target_link_libraries(net-ip-address ${GTEST_LIBRARIES} wiztk-net)