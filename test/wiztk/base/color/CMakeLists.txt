file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(base-color ${sources} ${headers})
target_link_libraries(base-color ${GTEST_LIBRARIES} wiztk)