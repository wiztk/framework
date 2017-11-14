file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(base-string ${sources} ${headers})
target_link_libraries(base-string ${GTEST_LIBRARIES} wiztk)