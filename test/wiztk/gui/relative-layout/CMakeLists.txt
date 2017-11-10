file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(gui-relative-layout ${sources} ${headers})
target_link_libraries(gui-relative-layout ${GTEST_LIBRARIES} wiztk)