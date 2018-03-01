file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(net-address-info ${sources} ${headers})
target_link_libraries(net-address-info ${GTEST_LIBRARIES} wiztk-net)