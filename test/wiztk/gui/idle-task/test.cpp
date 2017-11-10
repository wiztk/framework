//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <wiztk/gui/task.hpp>

using wiztk::gui::Task;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

/*
 *
 */
TEST_F(Test, SetNext1) {

  ASSERT_TRUE(true);
}

/*
 *
 */
TEST_F(Test, Unlink1) {
  ASSERT_TRUE(true);
}


/*
 *
 */
TEST_F(Test, SetNext2) {
  ASSERT_TRUE(true);
}

/*
 *
 */
TEST_F(Test, SetPrevious1) {
  ASSERT_TRUE(true);
}

/*
 *
 */
TEST_F(Test, SetPrevious2) {
  ASSERT_TRUE(true);
}

TEST_F(Test, PushBackLink1) {
  ASSERT_TRUE(true);
}

TEST_F(Test, PushFrontLink1) {
  ASSERT_TRUE(true);
}
