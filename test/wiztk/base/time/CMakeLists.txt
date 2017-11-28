file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(base-time ${sources} ${headers})
target_link_libraries(base-time ${GTEST_LIBRARIES} wiztk)