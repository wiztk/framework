file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(system-event-event-loop ${sources} ${headers})
target_link_libraries(system-event-event-loop ${GTEST_LIBRARIES} wiztk)