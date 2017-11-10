file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(base-trace ${sources} ${headers})
target_link_libraries(base-trace ${GTEST_LIBRARIES} wiztk)