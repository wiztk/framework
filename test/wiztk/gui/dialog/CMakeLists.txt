file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(gui-dialog ${sources} ${headers})
target_link_libraries(gui-dialog ${GTEST_LIBRARIES} wiztk)