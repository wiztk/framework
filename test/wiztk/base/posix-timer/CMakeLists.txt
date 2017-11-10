file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(base-posix-timer ${sources} ${headers})
target_link_libraries(base-posix-timer ${GTEST_LIBRARIES} wiztk)