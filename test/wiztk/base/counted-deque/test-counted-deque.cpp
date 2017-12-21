//
// Created by zhanggyb on 16-9-19.
//

#include "test-counted-deque.hpp"

#include <wiztk/base/counted-deque.hpp>

using namespace wiztk;
using namespace wiztk::base;

class MyElement : public CountedDeque::Element {

 public:

  explicit MyElement(int id)
      : id_(id) {}

  virtual ~MyElement() = default;

  int id() const { return id_; };

  MyElement *previous() const {
    return dynamic_cast<MyElement *>(CountedDeque::Element::previous());
  }

  MyElement *next() const {
    return dynamic_cast<MyElement *>(CountedDeque::Element::next());
  }

 private:

  int id_;

};

class MyCountedDeque : public CountedDeque {

 public:

  MyCountedDeque() = default;

  virtual ~MyCountedDeque() = default;

  CountedDeque::Element *_first() const {
    return first();
  }

  CountedDeque::Element *_last() const {
    return last();
  }

};

TEST_F(TestCountedDeque, push_front_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyCountedDeque deque;
  deque.PushFront(item1);
  deque.PushFront(item2);
  deque.PushFront(item3);

  ASSERT_TRUE(deque.count() == 3);
  ASSERT_TRUE(item1->next() == nullptr);
  ASSERT_TRUE(item1->previous() == item2);
  ASSERT_TRUE(item2->previous() == item3);
  ASSERT_TRUE(item3->previous() == nullptr);
  ASSERT_TRUE(deque._first() == item3);
  ASSERT_TRUE(deque._last() == item1);
}

TEST_F(TestCountedDeque, push_back_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyCountedDeque deque;
  deque.PushBack(item1);
  deque.PushBack(item2);
  deque.PushBack(item3);

  ASSERT_TRUE(deque.count() == 3);
  ASSERT_TRUE(item1->previous() == nullptr);
  ASSERT_TRUE(item1->next() == item2);
  ASSERT_TRUE(item2->next() == item3);
  ASSERT_TRUE(item3->next() == nullptr);
  ASSERT_TRUE(deque._first() == item1);
  ASSERT_TRUE(deque._last() == item3);
}

TEST_F(TestCountedDeque, insert_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyCountedDeque deque;
  deque.Insert(item1);
  deque.Insert(item2);
  deque.Insert(item3);

  ASSERT_TRUE(deque.count() == 3);
  ASSERT_TRUE(item1->next() == nullptr);
  ASSERT_TRUE(item1->previous() == item2);
  ASSERT_TRUE(item2->previous() == item3);
  ASSERT_TRUE(item3->previous() == nullptr);
}

TEST_F(TestCountedDeque, insert_2) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyCountedDeque deque;
  deque.Insert(item1);
  deque.Insert(item2);
  deque.Insert(item3);

  auto item4 = new MyElement(4);
  deque.Insert(item4);

  ASSERT_TRUE(deque.count() == 4);
  ASSERT_TRUE(item1->previous() == item2);
  ASSERT_TRUE(item2->previous() == item3);
  ASSERT_TRUE(item3->previous() == item4);
  ASSERT_TRUE(item4->previous() == nullptr);
}

TEST_F(TestCountedDeque, insert_3) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyCountedDeque deque;
  deque.Insert(item1);
  deque.Insert(item2);
  deque.Insert(item3);

  auto item4 = new MyElement(4);
  deque.Insert(item4, -1);

  ASSERT_TRUE(deque.count() == 4);
  ASSERT_TRUE(item1->previous() == item2);
  ASSERT_TRUE(item2->previous() == item3);
  ASSERT_TRUE(item3->previous() == nullptr);
  ASSERT_TRUE(item1->next() == item4);
  ASSERT_TRUE(item4->next() == nullptr);
}

class MyNode : public CountedBinode<MyNode> {};

TEST_F(TestCountedDeque, foo_1) {
  CountedDequeExt<MyNode> deque([](BinodeBase *obj) { delete obj; });

  auto item1 = new MyNode;
  auto item2 = new MyNode;
  auto item3 = new MyNode;

  deque.insert(item1);
  deque.insert(item2);
  deque.insert(item3);

  ASSERT_TRUE(deque.count() == 3);

  deque.clear();
  ASSERT_TRUE(deque.count() == 0);
}