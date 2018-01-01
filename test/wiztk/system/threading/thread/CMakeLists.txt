file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(system-threading-thread ${sources} ${headers})
target_link_libraries(system-threading-thread ${GTEST_LIBRARIES} wiztk)
