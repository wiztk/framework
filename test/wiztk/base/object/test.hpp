//
// Created by zhanggyb on 16-9-19.
//

#ifndef SKLAND_TEST_base_SIGCXX_TRACKABLE_HPP_
#define SKLAND_TEST_base_SIGCXX_TRACKABLE_HPP_

#include <gtest/gtest.h>

#include <wiztk/base/object.hpp>

using namespace wiztk;
using namespace wiztk::base;

class Test : public testing::Test {
 public:
  Test();
  virtual ~Test();

 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};

class TestableManager;
class TestableSubject : public Object {

  friend class TestableManager;

 public:

  inline TestableSubject()
      : Object(), manager_(nullptr) {

  }

  virtual ~TestableSubject();

  inline TestableSubject *previous_subject() const {
    return static_cast<TestableSubject *>(previous());
  }

  inline TestableSubject *next_subject() const {
    return static_cast<TestableSubject *>(next());
  }

 private:
  TestableManager *manager_;
};

class TestableManager : public Object {

  friend class TestableSubject;

 public:

  inline TestableManager()
      : Object(),
        first_subject_(nullptr),
        last_subject_(nullptr),
        subjects_count_(0) {
  }

  virtual ~TestableManager();

  void PushBackSubject(TestableSubject *object);

  void PushFrontSubject(TestableSubject *object);

  void InsertSubject(TestableSubject *object, int index = 0);

  void ClearSubjects();

  inline TestableSubject *first_subject() const {
    return first_subject_;
  }

  inline TestableSubject *last_subject() const {
    return last_subject_;
  }

  inline int subjects_count() const {
    return subjects_count_;
  }

 private:

  TestableSubject *first_subject_;
  TestableSubject *last_subject_;
  int subjects_count_;

};

#endif //WAYLAND_TOOLKIT_TEST_HPP
