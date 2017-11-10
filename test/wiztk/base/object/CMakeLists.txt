file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(base-object ${sources} ${headers})
target_link_libraries(base-object ${GTEST_LIBRARIES} wiztk)