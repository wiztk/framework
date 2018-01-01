file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(system-time-clock ${sources} ${headers})
target_link_libraries(system-time-clock ${GTEST_LIBRARIES} wiztk)