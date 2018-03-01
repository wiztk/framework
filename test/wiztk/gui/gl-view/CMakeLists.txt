file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(gui-gl-view ${sources} ${headers})
target_link_libraries(gui-gl-view ${GTEST_LIBRARIES} wiztk-gui)