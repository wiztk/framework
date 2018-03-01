file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(gui-slider ${sources} ${headers})
target_link_libraries(gui-slider ${GTEST_LIBRARIES} wiztk-gui)