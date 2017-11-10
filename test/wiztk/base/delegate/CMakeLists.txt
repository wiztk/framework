file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(base-delegate ${sources} ${headers})
target_link_libraries(base-delegate ${GTEST_LIBRARIES} wiztk)
