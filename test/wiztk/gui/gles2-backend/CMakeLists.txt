file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(gui-gles2-backend ${sources} ${headers})
target_link_libraries(gui-gles2-backend ${GTEST_LIBRARIES} wiztk-gui)