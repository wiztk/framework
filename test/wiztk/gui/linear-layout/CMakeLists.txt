file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(gui-linear-layout ${sources} ${headers})
target_link_libraries(gui-linear-layout ${GTEST_LIBRARIES} wiztk-gui)