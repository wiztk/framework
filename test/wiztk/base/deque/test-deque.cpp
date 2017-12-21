//
// Created by zhanggyb on 16-9-19.
//

#include "test-deque.hpp"

#include <wiztk/base/deque.hpp>

using namespace wiztk;
using namespace wiztk::base;

class MyElement : public BinodeBase {

 public:

  explicit MyElement(int id)
      : id_(id) {}

  ~MyElement() final = default;

  int id() const { return id_; };

  BinodeBase *previous() const { return previous_; }

  BinodeBase *next() const { return next_; }

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
  deque.PushFront(item1);
  deque.PushFront(item2);
  deque.PushFront(item3);

  ASSERT_TRUE(deque.GetCount() == 3);

  Deque<MyElement>::ConstIterator it = deque.crbegin();
  ASSERT_TRUE(it == item1);

  it = deque.cend();
  ASSERT_TRUE(it != item1);

  ASSERT_TRUE(item1->next() == deque.tail());
  ASSERT_TRUE(item1->previous() == item2);
  ASSERT_TRUE(item2->previous() == item3);
  ASSERT_TRUE(item3->previous() == deque.head());

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(TestDeque, push_back_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.PushBack(item1);
  deque.PushBack(item2);
  deque.PushBack(item3);

  ASSERT_TRUE(item1->previous() == deque.head());
  ASSERT_TRUE(item1->next() == item2);
  ASSERT_TRUE(item2->next() == item3);
  ASSERT_TRUE(item3->next() == deque.tail());

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(TestDeque, insert_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.Insert(item1);
  deque.Insert(item2);
  deque.Insert(item3);

  ASSERT_TRUE(item1->next() == deque.tail());
  ASSERT_TRUE(item1->previous() == item2);
  ASSERT_TRUE(item2->previous() == item3);
  ASSERT_TRUE(item3->previous() == deque.head());

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(TestDeque, insert_2) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque([](BinodeBase *obj) { delete obj; });
  deque.Insert(item1);
  deque.Insert(item2);
  deque.Insert(item3);

  auto item4 = new MyElement(4);
  deque.Insert(item4);

  ASSERT_TRUE(item1->previous() == item2);
  ASSERT_TRUE(item2->previous() == item3);
  ASSERT_TRUE(item3->previous() == item4);
  ASSERT_TRUE(item4->previous() == deque.head());
}

TEST_F(TestDeque, insert_3) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.Insert(item1);
  deque.Insert(item2);
  deque.Insert(item3);

  auto item4 = new MyElement(4);
  deque.Insert(item4, -1);

  ASSERT_TRUE(item1->previous() == item2);
  ASSERT_TRUE(item2->previous() == item3);
  ASSERT_TRUE(item3->previous() == deque.head());
  ASSERT_TRUE(item1->next() == item4);
  ASSERT_TRUE(item4->next() == deque.tail());

  delete item1;
  delete item2;
  delete item3;
  delete item4;

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(TestDeque, get_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.Insert(item3);
  deque.Insert(item2);
  deque.Insert(item1);

  BinodeBase *item = deque[0];
  ASSERT_TRUE(item = item1);

  item = deque[-1];
  ASSERT_TRUE(item = item3);

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(TestDeque, iterator_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.Insert(item3);
  deque.Insert(item2);
  deque.Insert(item1);

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

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(TestDeque, end_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.Insert(item3);
  deque.Insert(item2);
  deque.Insert(item1);

  MyDeque::Iterator it = deque.end();
  ASSERT_TRUE(it.get() == nullptr);

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(TestDeque, rend_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.Insert(item3);
  deque.Insert(item2);
  deque.Insert(item1);

  MyDeque::Iterator it = deque.rend();
  ASSERT_TRUE(it.get() == nullptr);

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(TestDeque, boolean_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.Insert(item3);
  deque.Insert(item2);
  deque.Insert(item1);

  MyDeque::Iterator it = deque.end();
  ASSERT_TRUE(!it);

  --it;
//  ASSERT_TRUE(it);

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(TestDeque, boolean_2) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.Insert(item3);
  deque.Insert(item2);
  deque.Insert(item1);

  MyDeque::Iterator it = deque.rend();
  ASSERT_TRUE(!it);

  ++it;
//  ASSERT_TRUE(it);

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(TestDeque, clear_1) {
  auto *item1 = new MyElement(1);
  auto *item2 = new MyElement(2);
  auto *item3 = new MyElement(3);

  MyDeque deque;
  deque.PushBack(item1);
  deque.PushBack(item2);
  deque.PushBack(item3);

  deque.Clear([](BinodeBase *obj) { delete obj; });

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(TestDeque, clear_2) {
  auto *item1 = new MyElement(1);
  auto *item2 = new MyElement(2);
  auto *item3 = new MyElement(3);

  MyDeque deque;
  deque.PushBack(item1);
  deque.PushBack(item2);
  deque.PushBack(item3);

  deque.Clear();

  ASSERT_TRUE(deque.IsEmpty());

  ASSERT_TRUE(!item1->IsLinked());
  ASSERT_TRUE(!item2->IsLinked());
  ASSERT_TRUE(!item3->IsLinked());

  delete item1;
  delete item2;
  delete item3;
}
