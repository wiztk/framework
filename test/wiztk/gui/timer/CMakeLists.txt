file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(gui-timer ${sources} ${headers})
target_link_libraries(gui-timer ${GTEST_LIBRARIES} wiztk)