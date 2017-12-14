file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(base-counted-deque ${sources} ${headers})
target_link_libraries(base-counted-deque ${GTEST_LIBRARIES} wiztk)