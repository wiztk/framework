file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(gui-window ${sources} ${headers})
target_link_libraries(gui-window ${GTEST_LIBRARIES} wiztk-gui)