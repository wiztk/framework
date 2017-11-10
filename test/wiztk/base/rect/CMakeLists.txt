file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(base-rect ${sources} ${headers})
target_link_libraries(base-rect ${GTEST_LIBRARIES} wiztk)