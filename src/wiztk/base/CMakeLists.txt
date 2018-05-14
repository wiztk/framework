# Copyright 2017 - 2018 The WizTK Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set(
        base_sources
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/memory/abstract-ref-counted.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/memory/atomic-ref-count.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/memory/ref-count.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/memory/ref-counted-base.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/memory/ref-counted-thread-safe-base.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/memory/shared-ptr.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/memory/singleton.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/memory/weak-ptr.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/abstract-callable.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/abstract-runnable.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/binode.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/bit.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/thickness.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/clamp.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/color.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/counted-deque.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/delegate.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/deque.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/dynamic-library.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/exception.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/macros.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/object.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/point.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/property.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/rect.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/sigcxx.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/size.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/string.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/trace.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/types.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/base/vector.hpp
        binode.cpp
        counted-deque.cpp
        dynamic-library.cpp
        object.cpp
        sigcxx.cpp
        string.cpp
        trace.cpp
)

if (BUILD_SHARED_LIBRARY)
    add_library(wiztk-base SHARED ${config_header} ${base_sources})
    set_target_properties(wiztk-base PROPERTIES VERSION 1 SOVERSION 1)
else ()
    add_library(wiztk-base ${config_header} ${base_sources})
endif ()

target_link_libraries(
        wiztk-base
        PUBLIC ${ICU_LIBRARIES}
)

install(TARGETS wiztk-base DESTINATION lib)
