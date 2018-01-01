file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(system-time-timer ${sources} ${headers})
target_link_libraries(system-time-timer ${GTEST_LIBRARIES} wiztk)