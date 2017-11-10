//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <wiztk/base/object.hpp>

using namespace wiztk;

class TestableObject : public Object {

 public:

  inline TestableObject() : Object() {}

  inline uint32_t get_children_count() const { return children_count(); }

  inline void push_front_child(TestableObject *obj) { PushFrontChild(obj); }

  inline void push_back_child(TestableObject *obj) { PushBackChild(obj); }

  inline void insert_child(TestableObject *obj, int pos = 0) { InsertChild(obj, pos); }

  inline void clear_children() { ClearChildren(); }

  inline Object *get_previous() const { return previous(); }

  inline Object *get_next() const { return next(); }

  inline Object *get_first_child() const { return first_child(); }

  inline Object *get_last_child() const { return last_child(); }

};

TestableSubject::~TestableSubject() {
  RemoveManagedObject(manager_,
                      this,
                      &manager_,
                      &manager_->first_subject_,
                      &manager_->last_subject_,
                      manager_->subjects_count_);
}

TestableManager::~TestableManager() {
  ClearManagedObject(this,
                     &first_subject_,
                     &last_subject_,
                     subjects_count_);
}

void TestableManager::PushBackSubject(TestableSubject *object) {
  PushBackManagedObject(this,
                        object,
                        &object->manager_,
                        &first_subject_,
                        &last_subject_,
                        subjects_count_);
}

void TestableManager::PushFrontSubject(TestableSubject *object) {
  PushFrontManagedObject(this,
                         object,
                         &object->manager_,
                         &first_subject_,
                         &last_subject_,
                         subjects_count_);
}

void TestableManager::InsertSubject(TestableSubject *object, int index) {
  InsertManagedObject(this,
                      object,
                      &object->manager_,
                      &first_subject_,
                      &last_subject_,
                      subjects_count_,
                      index);
}

void TestableManager::ClearSubjects() {
  ClearManagedObject(this,
                     &first_subject_,
                     &last_subject_,
                     subjects_count_);
}

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, construct_1) {
  TestableObject obj;

  ASSERT_TRUE(obj.get_children_count() == 0);
}

/**
 * Insert at 0, same as push front
 */
TEST_F(Test, insert_1) {
  TestableObject parent;
  TestableObject *child1 = new TestableObject;
  TestableObject *child2 = new TestableObject;
  TestableObject *child3 = new TestableObject;

  parent.insert_child(child1);
  parent.insert_child(child2);
  parent.insert_child(child3);

  ASSERT_TRUE(parent.get_children_count() == 3);

  ASSERT_TRUE(child3->get_next() == child2 && child2->get_next() == child1);

  parent.clear_children();
  ASSERT_TRUE(parent.get_children_count() == 0);
}

/**
 * Insert at end, same as push back
 */
TEST_F(Test, insert_2) {
  TestableObject parent;
  TestableObject *child1 = new TestableObject;
  TestableObject *child2 = new TestableObject;
  TestableObject *child3 = new TestableObject;

  parent.insert_child(child1, -1);
  parent.insert_child(child2, -1);
  parent.insert_child(child3, -1);

  ASSERT_TRUE(parent.get_children_count() == 3);

  ASSERT_TRUE(child1->get_next() == child2 && child2->get_next() == child3);

  parent.clear_children();
  ASSERT_TRUE(parent.get_children_count() == 0);
}

/**
 * Insert as arbitrary index
 */
TEST_F(Test, insert_3) {
  TestableObject parent;
  TestableObject *child1 = new TestableObject;
  TestableObject *child2 = new TestableObject;
  TestableObject *child3 = new TestableObject;
  TestableObject *child4 = new TestableObject;

  parent.insert_child(child2);
  parent.insert_child(child3, -1);
  parent.insert_child(child1, 0);

  ASSERT_TRUE(parent.get_children_count() == 3);
  ASSERT_TRUE(child1->get_next() == child2 && child2->get_next() == child3);

  parent.insert_child(child4, 1);
  ASSERT_TRUE(child1->get_next() == child4 && child4->get_previous() == child1 &&
      child4->get_next() == child2 && child2->get_previous() == child4);

  parent.clear_children();
  ASSERT_TRUE(parent.get_children_count() == 0);
}

TEST_F(Test, push_back1) {
  TestableObject parent;
  TestableObject *child1 = new TestableObject;
  TestableObject *child2 = new TestableObject;
  TestableObject *child3 = new TestableObject;

  parent.push_back_child(child1);
  parent.push_back_child(child2);
  parent.push_back_child(child3);

  ASSERT_TRUE(child1->get_next() == child2 && child2->get_next() == child3 && parent.get_children_count() == 3);

  parent.clear_children();
  ASSERT_TRUE(parent.get_children_count() == 0);
}

TEST_F(Test, clear_1) {
  TestableObject parent;
  TestableObject *child1 = new TestableObject;
  TestableObject *child2 = new TestableObject;
  TestableObject *child3 = new TestableObject;

  parent.push_back_child(child1);
  parent.push_back_child(child2);
  parent.push_back_child(child3);

  ASSERT_TRUE(parent.get_children_count() == 3);
  parent.clear_children();
  ASSERT_TRUE(parent.get_children_count() == 0);
}

TEST_F(Test, push_back_managed_object_1) {
  TestableManager manager;
  TestableSubject *subject1 = new TestableSubject();
  TestableSubject *subject2 = new TestableSubject();
  TestableSubject *subject3 = new TestableSubject();

  manager.PushBackSubject(subject1);
  manager.PushBackSubject(subject2);
  manager.PushBackSubject(subject3);

  ASSERT_TRUE(manager.subjects_count() == 3 &&
      subject1->next_subject() == subject2 &&
      subject2->next_subject() == subject3 &&
      manager.first_subject() == subject1 &&
      manager.last_subject() == subject3);
}

TEST_F(Test, push_front_managed_object_1) {
  TestableManager manager;
  TestableSubject *subject1 = new TestableSubject();
  TestableSubject *subject2 = new TestableSubject();
  TestableSubject *subject3 = new TestableSubject();

  manager.PushFrontSubject(subject3);
  manager.PushFrontSubject(subject2);
  manager.PushFrontSubject(subject1);

  ASSERT_TRUE(manager.subjects_count() == 3 &&
      subject1->next_subject() == subject2 &&
      subject2->next_subject() == subject3 &&
      manager.first_subject() == subject1 &&
      manager.last_subject() == subject3);
}

TEST_F(Test, insert_managed_object_1) {
  TestableManager manager;
  TestableSubject *subject1 = new TestableSubject();
  TestableSubject *subject2 = new TestableSubject();
  TestableSubject *subject3 = new TestableSubject();

  manager.InsertSubject(subject3);
  manager.InsertSubject(subject2);
  manager.InsertSubject(subject1);

  ASSERT_TRUE(manager.subjects_count() == 3 &&
      subject1->next_subject() == subject2 &&
      subject2->next_subject() == subject3 &&
      manager.first_subject() == subject1 &&
      manager.last_subject() == subject3);
}

TEST_F(Test, clear_managed_objects_1) {
  TestableManager manager;
  TestableSubject *subject1 = new TestableSubject();
  TestableSubject *subject2 = new TestableSubject();
  TestableSubject *subject3 = new TestableSubject();

  manager.PushBackSubject(subject1);
  manager.PushBackSubject(subject2);
  manager.PushBackSubject(subject3);

  manager.ClearSubjects();

  ASSERT_TRUE(manager.subjects_count() == 0);
}