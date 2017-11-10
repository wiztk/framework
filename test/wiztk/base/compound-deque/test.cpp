//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <wiztk/base/compound-deque.hpp>

using namespace wiztk;
using namespace wiztk::base;

class Item : public CompoundDeque::Element {

 public:

  explicit Item(int id)
      : id_(id) {}

  virtual ~Item() = default;

  int id() const { return id_; };

  Item *_previous() const {
    return dynamic_cast<Item *>(CompoundDeque::Element::previous());
  }

  Item *_next() const {
    return dynamic_cast<Item *>(CompoundDeque::Element::next());
  }

 private:

  int id_;

};

class TestDeque : public CompoundDeque {

 public:

  TestDeque() = default;

  virtual ~TestDeque() = default;

  CompoundDeque::Element* _first() const {
    return first();
  }

  CompoundDeque::Element* _last() const {
    return last();
  }

};

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, push_front_1) {
  auto item1 = new Item(1);
  auto item2 = new Item(2);
  auto item3 = new Item(3);

  TestDeque deque;
  deque.PushFront(item1);
  deque.PushFront(item2);
  deque.PushFront(item3);

  ASSERT_TRUE(deque.count() == 3);
  ASSERT_TRUE(item1->_next() == nullptr);
  ASSERT_TRUE(item1->_previous() == item2);
  ASSERT_TRUE(item2->_previous() == item3);
  ASSERT_TRUE(item3->_previous() == nullptr);
  ASSERT_TRUE(deque._first() == item3);
  ASSERT_TRUE(deque._last() == item1);
}

TEST_F(Test, push_back_1) {
  auto item1 = new Item(1);
  auto item2 = new Item(2);
  auto item3 = new Item(3);

  TestDeque deque;
  deque.PushBack(item1);
  deque.PushBack(item2);
  deque.PushBack(item3);

  ASSERT_TRUE(deque.count() == 3);
  ASSERT_TRUE(item1->_previous() == nullptr);
  ASSERT_TRUE(item1->_next() == item2);
  ASSERT_TRUE(item2->_next() == item3);
  ASSERT_TRUE(item3->_next() == nullptr);
  ASSERT_TRUE(deque._first() == item1);
  ASSERT_TRUE(deque._last() == item3);
}

TEST_F(Test, insert_1) {
  auto item1 = new Item(1);
  auto item2 = new Item(2);
  auto item3 = new Item(3);

  TestDeque deque;
  deque.Insert(item1);
  deque.Insert(item2);
  deque.Insert(item3);

  ASSERT_TRUE(deque.count() == 3);
  ASSERT_TRUE(item1->_next() == nullptr);
  ASSERT_TRUE(item1->_previous() == item2);
  ASSERT_TRUE(item2->_previous() == item3);
  ASSERT_TRUE(item3->_previous() == nullptr);
}

TEST_F(Test, insert_2) {
  auto item1 = new Item(1);
  auto item2 = new Item(2);
  auto item3 = new Item(3);

  TestDeque deque;
  deque.Insert(item1);
  deque.Insert(item2);
  deque.Insert(item3);

  auto item4 = new Item(4);
  deque.Insert(item4);

  ASSERT_TRUE(deque.count() == 4);
  ASSERT_TRUE(item1->_previous() == item2);
  ASSERT_TRUE(item2->_previous() == item3);
  ASSERT_TRUE(item3->_previous() == item4);
  ASSERT_TRUE(item4->_previous() == nullptr);
}

TEST_F(Test, insert_3) {
  auto item1 = new Item(1);
  auto item2 = new Item(2);
  auto item3 = new Item(3);

  TestDeque deque;
  deque.Insert(item1);
  deque.Insert(item2);
  deque.Insert(item3);

  auto item4 = new Item(4);
  deque.Insert(item4, -1);

  ASSERT_TRUE(deque.count() == 4);
  ASSERT_TRUE(item1->_previous() == item2);
  ASSERT_TRUE(item2->_previous() == item3);
  ASSERT_TRUE(item3->_previous() == nullptr);
  ASSERT_TRUE(item1->_next() == item4);
  ASSERT_TRUE(item4->_next() == nullptr);
}
