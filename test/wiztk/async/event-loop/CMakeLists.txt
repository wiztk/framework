file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(async-event-loop ${sources} ${headers})
target_link_libraries(async-event-loop ${GTEST_LIBRARIES} wiztk)