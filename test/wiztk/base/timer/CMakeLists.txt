file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(base-timer ${sources} ${headers})
target_link_libraries(base-timer ${GTEST_LIBRARIES} wiztk)