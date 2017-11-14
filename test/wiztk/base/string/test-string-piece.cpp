//
// Created by zhanggyb on 17-11-14.
//

#include "test-string-piece.hpp"

#include "wiztk/base/string.hpp"

using namespace wiztk;
using namespace wiztk::base;

// Constructor test

TEST_F(TestStringPiece, constructor_1) {
  std::string str("Hello World!");

  StringPiece str_piece(str);

  ASSERT_TRUE(str_piece.length() == str.length());
}

TEST_F(TestStringPiece, constructor_2) {
  const char *str = "Hello World!";

  StringPiece str_piece(str);

  ASSERT_TRUE(str_piece.length() == strlen(str));
}

TEST_F(TestStringPiece, constructor_3) {
  const char *str = "Hello World!";

  StringPiece str_piece(str + 6, 5);

  ASSERT_TRUE(str_piece.front() == 'W' && str_piece.back() == 'd');
}

TEST_F(TestStringPiece, constructor_4) {
  const char *str = "Hello World!";

  StringPiece str_piece1(str);
  StringPiece str_piece2(str_piece1, 6);

  ASSERT_TRUE(str_piece2.front() == 'W' && str_piece2.back() == '!' && str_piece2.length() == 6);
}

TEST_F(TestStringPiece, constructor_5) {
  const char *str = "Hello World!";

  StringPiece str_piece1(str);
  StringPiece str_piece2(str_piece1, 6, 5);

  ASSERT_TRUE(str_piece2[0] == 'W' && str_piece2[4] == 'd' && str_piece2.length() == 5);
}

// StringPieceT::compare()

TEST_F(TestStringPiece, compare_1) {
  std::string str("Hello World!");

  StringPiece str_piece1(str);
  StringPiece str_piece2(str);

  ASSERT_TRUE(str_piece1 == str_piece2);
}

TEST_F(TestStringPiece, compare_2) {
  std::string str("Hello World!");

  StringPiece str_piece1(str);
  StringPiece str_piece2(str, 2);

  ASSERT_TRUE(str_piece1 != str_piece2);
}

// StringPieceT::copy_to()

TEST_F(TestStringPiece, copy_to_1) {
  std::string src("Hello World!");
  std::string dst("XXXXXXXXXXXX");

  StringPiece str_piece(src);
  str_piece.copy_to(&dst);

  ASSERT_TRUE(dst == src);
}

// Find:

TEST_F(TestStringPiece, find_1) {
  StringPiece str("Hello World!");
  StringPiece s("Wor");

  size_t ret = str.find(s);

  ASSERT_TRUE(ret == 6);
}

TEST_F(TestStringPiece, find_2) {
  StringPiece str("Hello World!");
  StringPiece s("Wor");

  size_t ret = str.find(s, 7);

  ASSERT_TRUE(ret == StringPiece::npos);
}

TEST_F(TestStringPiece, find_3) {
  StringPiece str("Hello World!");

  size_t ret = str.find("W");

  ASSERT_TRUE(ret == 6);
}

TEST_F(TestStringPiece, find_4) {
  StringPiece str("Hello World!");

  size_t ret = str.find("W", 7);

  ASSERT_TRUE(ret == StringPiece::npos);
}

// RFind:

TEST_F(TestStringPiece, rfind_1) {
  StringPiece str("Hello World!");
  StringPiece s("Wor");

  size_t ret = str.rfind(s);

  ASSERT_TRUE(ret == 6);
}

TEST_F(TestStringPiece, rfind_2) {
  StringPiece str("Hello World!");
  StringPiece s("Wor");

  size_t ret = str.rfind(s, 7);

  ASSERT_TRUE(ret == 6);
}

TEST_F(TestStringPiece, rfind_3) {
  StringPiece str("Hello World!");

  size_t ret = str.rfind("W");

  ASSERT_TRUE(ret == 6);
}

TEST_F(TestStringPiece, rfind_4) {
  StringPiece str("Hello World!");

  size_t ret = str.rfind("W", 7);

  ASSERT_TRUE(ret == 6);
}

// Find first:

TEST_F(TestStringPiece, find_first_of_1) {
  std::string str("Hello World!");
  std::string s("World");

  StringPiece str_piece1(str);
  StringPiece str_piece2(s);

  size_t a = str_piece1.find_first_of(str_piece2);

  ASSERT_TRUE(0 != a);
}
