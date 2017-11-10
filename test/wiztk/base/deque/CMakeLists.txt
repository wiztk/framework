file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(base-deque ${sources} ${headers})
target_link_libraries(base-deque ${GTEST_LIBRARIES} wiztk)