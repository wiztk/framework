//
// Created by zhanggyb on 16-9-19.
//

#include "test-deque.hpp"

#include <wiztk/base/deque.hpp>

using namespace wiztk;
using namespace wiztk::base;

class MyElement : public DequeNode<MyElement> {

 public:

  explicit MyElement(int id)
      : id_(id) {}

  ~MyElement() final = default;

  int id() const { return id_; };

 private:

  int id_;

};

class MyDeque : public Deque<MyElement> {

 public:

  MyDeque() = default;

  explicit MyDeque(const DeleterType &deleter)
      : Deque<MyElement>(deleter) {}

  ~MyDeque() final = default;

  const BinodeBase *head() const { return &head_; }

  const BinodeBase *tail() const { return &tail_; }

};

TEST_F(TestDeque, push_front_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.push_front(item1);
  deque.push_front(item2);
  deque.push_front(item3);

  ASSERT_TRUE(deque.count() == 3);

  Deque<MyElement>::ConstIterator it = deque.crbegin();
  ASSERT_TRUE(it == item1);

  it = deque.cend();
  ASSERT_TRUE(it != item1);

  ASSERT_TRUE(item1->next() == nullptr);
  ASSERT_TRUE(item1->previous() == item2);
  ASSERT_TRUE(item2->previous() == item3);
  ASSERT_TRUE(item3->previous() == nullptr);

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.is_empty());
}

TEST_F(TestDeque, push_back_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.push_back(item1);
  deque.push_back(item2);
  deque.push_back(item3);

  ASSERT_TRUE(item1->previous() == nullptr);
  ASSERT_TRUE(item1->next() == item2);
  ASSERT_TRUE(item2->next() == item3);
  ASSERT_TRUE(item3->next() == nullptr);

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.is_empty());
}

TEST_F(TestDeque, insert_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.insert(item1);
  deque.insert(item2);
  deque.insert(item3);

  ASSERT_TRUE(item1->next() == nullptr);
  ASSERT_TRUE(item1->previous() == item2);
  ASSERT_TRUE(item2->previous() == item3);
  ASSERT_TRUE(item3->previous() == nullptr);

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.is_empty());
}

TEST_F(TestDeque, insert_2) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque([](BinodeBase *obj) { delete obj; });
  deque.insert(item1);
  deque.insert(item2);
  deque.insert(item3);

  auto item4 = new MyElement(4);
  deque.insert(item4);

  ASSERT_TRUE(item1->previous() == item2);
  ASSERT_TRUE(item2->previous() == item3);
  ASSERT_TRUE(item3->previous() == item4);
  ASSERT_TRUE(item4->previous() == nullptr);
}

TEST_F(TestDeque, insert_3) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.insert(item1);
  deque.insert(item2);
  deque.insert(item3);

  auto item4 = new MyElement(4);
  deque.insert(item4, -1);

  ASSERT_TRUE(item1->previous() == item2);
  ASSERT_TRUE(item2->previous() == item3);
  ASSERT_TRUE(item3->previous() == nullptr);
  ASSERT_TRUE(item1->next() == item4);
  ASSERT_TRUE(item4->next() == nullptr);

  delete item1;
  delete item2;
  delete item3;
  delete item4;

  ASSERT_TRUE(deque.is_empty());
}

TEST_F(TestDeque, get_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.insert(item3);
  deque.insert(item2);
  deque.insert(item1);

  BinodeBase *item = deque[0];
  ASSERT_TRUE(item == item1);

  item = deque[-1];
  ASSERT_TRUE(item == item3);

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.is_empty());
}

TEST_F(TestDeque, iterator_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.insert(item3);
  deque.insert(item2);
  deque.insert(item1);

  MyDeque::Iterator it = deque.begin();

  ASSERT_TRUE(it.get() == item1);
  ++it;
  ASSERT_TRUE(it.get() == item2);
  ++it;
  ASSERT_TRUE(it.get() == item3);
  ++it;
  ASSERT_TRUE(it == deque.end());

  it = deque.rbegin();
  ASSERT_TRUE(it.get() == item3);
  --it;
  ASSERT_TRUE(it.get() == item2);
  --it;
  ASSERT_TRUE(it.get() == item1);
  --it;
  ASSERT_TRUE(it == deque.rend());

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.is_empty());
}

TEST_F(TestDeque, end_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.insert(item3);
  deque.insert(item2);
  deque.insert(item1);

  MyDeque::Iterator it = deque.end();
  ASSERT_TRUE(it.get() == nullptr);

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.is_empty());
}

TEST_F(TestDeque, rend_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.insert(item3);
  deque.insert(item2);
  deque.insert(item1);

  MyDeque::Iterator it = deque.rend();
  ASSERT_TRUE(it.get() == nullptr);

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.is_empty());
}

TEST_F(TestDeque, boolean_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.insert(item3);
  deque.insert(item2);
  deque.insert(item1);

  MyDeque::Iterator it = deque.end();
  ASSERT_TRUE(!it);

  --it;
//  ASSERT_TRUE(it);

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.is_empty());
}

TEST_F(TestDeque, boolean_2) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.insert(item3);
  deque.insert(item2);
  deque.insert(item1);

  MyDeque::Iterator it = deque.rend();
  ASSERT_TRUE(!it);

  ++it;
//  ASSERT_TRUE(it);

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.is_empty());
}

TEST_F(TestDeque, clear_1) {
  auto *item1 = new MyElement(1);
  auto *item2 = new MyElement(2);
  auto *item3 = new MyElement(3);

  MyDeque deque;
  deque.push_back(item1);
  deque.push_back(item2);
  deque.push_back(item3);

  deque.clear([](BinodeBase *obj) { delete obj; });

  ASSERT_TRUE(deque.is_empty());
}

TEST_F(TestDeque, clear_2) {
  auto *item1 = new MyElement(1);
  auto *item2 = new MyElement(2);
  auto *item3 = new MyElement(3);

  MyDeque deque;
  deque.push_back(item1);
  deque.push_back(item2);
  deque.push_back(item3);

  deque.clear();

  ASSERT_TRUE(deque.is_empty());

  ASSERT_TRUE(!item1->is_linked());
  ASSERT_TRUE(!item2->is_linked());
  ASSERT_TRUE(!item3->is_linked());

  delete item1;
  delete item2;
  delete item3;
}

TEST_F(TestDeque, clear_3) {
  auto *item1 = new MyElement(1);
  auto *item2 = new MyElement(2);
  auto *item3 = new MyElement(3);

  MyDeque deque;
  deque.push_back(item1);
  deque.push_back(item2);
  deque.push_back(item3);

  deque.clear();

  ASSERT_TRUE(deque.is_empty());
  ASSERT_TRUE(!item1->is_linked());
  ASSERT_TRUE(!item2->is_linked());
  ASSERT_TRUE(!item3->is_linked());

  delete item1;
  delete item2;
  delete item3;
}
