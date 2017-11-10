file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(base-margin ${sources} ${headers})
target_link_libraries(base-margin ${GTEST_LIBRARIES} wiztk)