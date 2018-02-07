/*
 * Copyright 2017 - 2018 The WizTK Authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WIZTK_TEST_BASE_MEMORY_SINGLETON_HPP_
#define WIZTK_TEST_BASE_MEMORY_SINGLETON_HPP_

#include <gtest/gtest.h>

class TestSingleton : public testing::Test {

 public:

  TestSingleton() = default;

  ~TestSingleton() override = default;

 protected:

  void SetUp() final {}

  void TearDown() final {}

};

#endif // WIZTK_TEST_BASE_MEMORY_SINGLETON_HPP_
