file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(system-logging-logger ${sources} ${headers})
target_link_libraries(system-logging-logger ${GTEST_LIBRARIES} wiztk)