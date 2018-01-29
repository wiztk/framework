file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(system-threading-thread-local ${sources} ${headers})
target_link_libraries(system-threading-thread-local ${GTEST_LIBRARIES} wiztk)
