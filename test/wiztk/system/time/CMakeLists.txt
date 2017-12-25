file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(system-time ${sources} ${headers})
target_link_libraries(system-time ${GTEST_LIBRARIES} wiztk)