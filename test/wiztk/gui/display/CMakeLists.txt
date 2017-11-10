file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(gui-display ${sources} ${headers})
target_link_libraries(gui-display ${GTEST_LIBRARIES} wiztk)