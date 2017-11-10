file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(base-sigcxx ${sources} ${headers})
target_link_libraries(base-sigcxx ${GTEST_LIBRARIES} wiztk)