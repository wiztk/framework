//
// Created by zhanggyb on 16-9-19.
//

#include "test-deque.hpp"

#include <wiztk/base/deque.hpp>

using namespace wiztk;
using namespace wiztk::base;

class MyElement : public BiNode {

 public:

  explicit MyElement(int id)
      : id_(id) {}

  ~MyElement() final = default;

  int id() const { return id_; };

  BiNode *_previous() const { return previous(); }

  BiNode *_next() const { return next(); }

 private:

  int id_;

};

class MyDeque : public Deque<MyElement> {

 public:

  MyDeque() = default;

  ~MyDeque() final = default;

  const BiNode *_first() const { return first(); }

  const BiNode *_last() const { return last(); }

};

TEST_F(Test, push_front_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.PushFront(item1);
  deque.PushFront(item2);
  deque.PushFront(item3);

  ASSERT_TRUE(deque.GetSize() == 3);

  Deque<MyElement>::ConstIterator it = deque.crbegin();
  ASSERT_TRUE(it == item1);

  it = deque.cend();
  ASSERT_TRUE(it != item1);

  ASSERT_TRUE(item1->_next() == deque._last());
  ASSERT_TRUE(item1->_previous() == item2);
  ASSERT_TRUE(item2->_previous() == item3);
  ASSERT_TRUE(item3->_previous() == deque._first());

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(Test, push_back_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.PushBack(item1);
  deque.PushBack(item2);
  deque.PushBack(item3);

  ASSERT_TRUE(item1->_previous() == deque._first());
  ASSERT_TRUE(item1->_next() == item2);
  ASSERT_TRUE(item2->_next() == item3);
  ASSERT_TRUE(item3->_next() == deque._last());

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(Test, insert_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.Insert(item1);
  deque.Insert(item2);
  deque.Insert(item3);

  ASSERT_TRUE(item1->_next() == deque._last());
  ASSERT_TRUE(item1->_previous() == item2);
  ASSERT_TRUE(item2->_previous() == item3);
  ASSERT_TRUE(item3->_previous() == deque._first());

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(Test, insert_2) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.Insert(item1);
  deque.Insert(item2);
  deque.Insert(item3);

  auto item4 = new MyElement(4);
  deque.Insert(item4);

  ASSERT_TRUE(item1->_previous() == item2);
  ASSERT_TRUE(item2->_previous() == item3);
  ASSERT_TRUE(item3->_previous() == item4);
  ASSERT_TRUE(item4->_previous() == deque._first());
}

TEST_F(Test, insert_3) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.Insert(item1);
  deque.Insert(item2);
  deque.Insert(item3);

  auto item4 = new MyElement(4);
  deque.Insert(item4, -1);

  ASSERT_TRUE(item1->_previous() == item2);
  ASSERT_TRUE(item2->_previous() == item3);
  ASSERT_TRUE(item3->_previous() == deque._first());
  ASSERT_TRUE(item1->_next() == item4);
  ASSERT_TRUE(item4->_next() == deque._last());

  delete item1;
  delete item2;
  delete item3;
  delete item4;

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(Test, get_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.Insert(item3);
  deque.Insert(item2);
  deque.Insert(item1);

  BiNode *item = deque[0];
  ASSERT_TRUE(item = item1);

  item = deque[-1];
  ASSERT_TRUE(item = item3);

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(Test, iterator_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.Insert(item3);
  deque.Insert(item2);
  deque.Insert(item1);

  MyDeque::Iterator it = deque.begin();

  ASSERT_TRUE(it.element() == item1);
  ++it;
  ASSERT_TRUE(it.element() == item2);
  ++it;
  ASSERT_TRUE(it.element() == item3);
  ++it;
  ASSERT_TRUE(it == deque.end());

  it = deque.rbegin();
  ASSERT_TRUE(it.element() == item3);
  --it;
  ASSERT_TRUE(it.element() == item2);
  --it;
  ASSERT_TRUE(it.element() == item1);
  --it;
  ASSERT_TRUE(it == deque.rend());

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(Test, end_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.Insert(item3);
  deque.Insert(item2);
  deque.Insert(item1);

  MyDeque::Iterator it = deque.end();
  ASSERT_TRUE(it.element() == nullptr);

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(Test, rend_1) {
  auto item1 = new MyElement(1);
  auto item2 = new MyElement(2);
  auto item3 = new MyElement(3);

  MyDeque deque;
  deque.Insert(item3);
  deque.Insert(item2);
  deque.Insert(item1);

  MyDeque::Iterator it = deque.rend();
  ASSERT_TRUE(it.element() == nullptr);

  delete item1;
  delete item2;
  delete item3;

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(Test, boolean_1) {
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

TEST_F(Test, boolean_2) {
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

TEST_F(Test, clear_1) {
  auto *item1 = new MyElement(1);
  auto *item2 = new MyElement(2);
  auto *item3 = new MyElement(3);

  MyDeque deque;
  deque.PushBack(item1);
  deque.PushBack(item2);
  deque.PushBack(item3);

  deque.Clear([](BiNode *p) { delete p; });

  ASSERT_TRUE(deque.IsEmpty());
}

TEST_F(Test, clear_2) {
  auto *item1 = new MyElement(1);
  auto *item2 = new MyElement(2);
  auto *item3 = new MyElement(3);

  MyDeque deque;
  deque.PushBack(item1);
  deque.PushBack(item2);
  deque.PushBack(item3);

  deque.Clear([](BiNode *p) {/* Do nothing. */});

  ASSERT_TRUE(deque.IsEmpty());

  ASSERT_TRUE(!item1->IsLinked());
  ASSERT_TRUE(!item2->IsLinked());
  ASSERT_TRUE(!item3->IsLinked());

  delete item1;
  delete item2;
  delete item3;
}
