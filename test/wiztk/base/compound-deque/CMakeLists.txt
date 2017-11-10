file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(base-compound-deque ${sources} ${headers})
target_link_libraries(base-compound-deque ${GTEST_LIBRARIES} wiztk)