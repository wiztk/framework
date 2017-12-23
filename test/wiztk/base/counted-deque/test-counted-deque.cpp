//
// Created by zhanggyb on 16-9-19.
//

#include "test-counted-deque.hpp"

#include <wiztk/base/counted-deque.hpp>

using namespace wiztk;
using namespace wiztk::base;

class MyNode : public CountedDequeNode<MyNode> {};

TEST_F(TestCountedDeque, push_front_1) {
  auto item1 = new MyNode;
  auto item2 = new MyNode;
  auto item3 = new MyNode;

  CountedDeque<MyNode> deque([](BinodeBase *obj) { delete obj; });
  deque.push_front(item1);
  deque.push_front(item2);
  deque.push_front(item3);

  ASSERT_TRUE(deque.count() == 3);
  ASSERT_TRUE(item1->previous() == item2);
  ASSERT_TRUE(item2->previous() == item3);
}

TEST_F(TestCountedDeque, push_back_1) {
  auto item1 = new MyNode;
  auto item2 = new MyNode;
  auto item3 = new MyNode;

  CountedDeque<MyNode> deque([](BinodeBase *obj) { delete obj; });

  deque.push_back(item1);
  deque.push_back(item2);
  deque.push_back(item3);

  ASSERT_TRUE(deque.count() == 3);
  ASSERT_TRUE(item1->next() == item2);
  ASSERT_TRUE(item2->next() == item3);
}

TEST_F(TestCountedDeque, insert_1) {
  auto item1 = new MyNode;
  auto item2 = new MyNode;
  auto item3 = new MyNode;

  CountedDeque<MyNode> deque([](BinodeBase *obj) { delete obj; });

  deque.insert(item1);
  deque.insert(item2);
  deque.insert(item3);

  ASSERT_TRUE(deque.count() == 3);
//  ASSERT_TRUE(item1->next() == nullptr);
  ASSERT_TRUE(item1->previous() == item2);
  ASSERT_TRUE(item2->previous() == item3);
//  ASSERT_TRUE(item3->previous() == nullptr);
}

TEST_F(TestCountedDeque, insert_2) {
  auto item1 = new MyNode;
  auto item2 = new MyNode;
  auto item3 = new MyNode;

  CountedDeque<MyNode> deque([](BinodeBase *obj) { delete obj; });

  deque.insert(item1);
  deque.insert(item2);
  deque.insert(item3);

  auto item4 = new MyNode;
  deque.insert(item4);

  ASSERT_TRUE(deque.count() == 4);
  ASSERT_TRUE(item1->previous() == item2);
  ASSERT_TRUE(item2->previous() == item3);
  ASSERT_TRUE(item3->previous() == item4);
//  ASSERT_TRUE(item4->previous() == nullptr);
}

TEST_F(TestCountedDeque, insert_3) {
  auto item1 = new MyNode;
  auto item2 = new MyNode;
  auto item3 = new MyNode;

  CountedDeque<MyNode> deque([](BinodeBase *obj) { delete obj; });

  deque.insert(item1);
  deque.insert(item2);
  deque.insert(item3);

  auto item4 = new MyNode;
  deque.insert(item4, -1);

  ASSERT_TRUE(deque.count() == 4);
  ASSERT_TRUE(item1->previous() == item2);
  ASSERT_TRUE(item2->previous() == item3);
  ASSERT_TRUE(item3->previous() == nullptr);
  ASSERT_TRUE(item1->next() == item4);
  ASSERT_TRUE(item4->next() == nullptr);
}

TEST_F(TestCountedDeque, clear_1) {
  typedef CountedDeque<MyNode> D;
  D deque([](BinodeBase *obj) { delete obj; });

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

TEST_F(TestCountedDeque, clear_2) {
  typedef CountedDeque<MyNode> D;
  D deque;

  auto item1 = new MyNode;
  auto item2 = new MyNode;
  auto item3 = new MyNode;

  deque.insert(item1);
  deque.insert(item2);
  deque.insert(item3);

  ASSERT_TRUE(deque.count() == 3);

  deque.clear();
  ASSERT_TRUE(deque.count() == 0);

  ASSERT_TRUE(!item1->is_linked());
  ASSERT_TRUE(item1->deque() == nullptr);
  ASSERT_TRUE(!item2->is_linked());
  ASSERT_TRUE(item2->deque() == nullptr);
  ASSERT_TRUE(!item3->is_linked());
  ASSERT_TRUE(item3->deque() == nullptr);

  delete item1;
  delete item2;
  delete item3;
}
