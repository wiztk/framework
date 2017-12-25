file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(system-timer ${sources} ${headers})
target_link_libraries(system-timer ${GTEST_LIBRARIES} wiztk)