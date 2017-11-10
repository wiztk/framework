file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(base-vectors ${sources} ${headers})
target_link_libraries(base-vectors ${GTEST_LIBRARIES} wiztk)