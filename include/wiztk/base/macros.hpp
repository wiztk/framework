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

#ifndef WIZTK_BASE_DEFINES_HPP_
#define WIZTK_BASE_DEFINES_HPP_

#ifdef __DEBUG__

//#ifdef __UNIX__
#include <string.h>
#define FILE_BASE_NAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
//#else
//#include <string.h>
//#define FILE_BASE_FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
//#endif

#include <stdio.h>
#define _DEBUG(fmt, args...) \
    do { \
        fprintf(stderr, "%s:%d:%s(): " fmt, FILE_BASE_NAME, __LINE__, __FUNCTION__, args); \
    } while (false)

#include <cassert>
#define _ASSERT(expr) \
  do { \
    assert(expr); \
  } while (false)

#else // NOT __DEBUG__

#define _DEBUG(fmt, args...) ((void)0)
#define _ASSERT(expr) ((void)0)

#endif // END __DEBUG__

#define WIZTK_DISABLE_COPY_CONSTRUCTOR(CLASS) CLASS(const CLASS&) = delete
#define WIZTK_DISABLE_COPY_ASSIGNMENT(CLASS) CLASS& operator=(const CLASS&) = delete
#define WIZTK_DISABLE_MOVE_CONSTRUCTOR(CLASS) CLASS(CLASS&&) = delete
#define WIZTK_DISABLE_MOVE_ASSIGNMENT(CLASS) CLASS& operator=(CLASS&&) = delete

#define WIZTK_DECLARE_NONCOPYABLE(CLASS) \
  CLASS (const CLASS&) = delete; \
  CLASS& operator = (const CLASS&) = delete

#define WIZTK_DECLARE_NONMOVABLE(CLASS) \
  CLASS (CLASS&&) = delete; \
  CLASS& operator = (CLASS&&) = delete

#define WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(CLASS) \
  CLASS (const CLASS&) = delete; \
  CLASS& operator = (const CLASS&) = delete; \
  CLASS (CLASS&&) = delete; \
  CLASS& operator = (CLASS&&) = delete

#ifdef WIZTK_SHARED_EXPORT
#define WIZTK_EXPORT __attribute__((visibility("default")))
#define WIZTK_NO_EXPORT __attribute__((visibility("hidden")))
#else
#define WIZTK_EXPORT
#define WIZTK_NO_EXPORT
#endif  // WIZTK_SHARED_EXPORT

#ifndef WIZTK_DEPRECATED
#define WIZTK_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef WIZTK_DEPRECATED_EXPORT
#define WIZTK_DEPRECATED_EXPORT WIZTK_EXPORT WIZTK_DEPRECATED
#endif

#ifndef WIZTK_DEPRECATED_NO_EXPORT
#define WIZTK_DEPRECATED_NO_EXPORT WIZTK_NO_EXPORT WIZTK_DEPRECATED
#endif

#endif // WIZTK_BASE_DEFINES_HPP_
