file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(gui-idle-task ${sources} ${headers})
target_link_libraries(gui-idle-task ${GTEST_LIBRARIES} wiztk)
