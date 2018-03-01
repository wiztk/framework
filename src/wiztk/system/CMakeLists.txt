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
        system_sources
        ${PROJECT_SOURCE_DIR}/include/wiztk/system/logging/logger.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/system/logging/type.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/system/threading/main-thread.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/system/threading/mutex.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/system/threading/thread.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/system/threading/thread-local.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/system/threading/thread-state.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/system/time/clock.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/system/time/delta.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/system/time/realtime-clock.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/system/time/timer.hpp
        logging/logger.cpp
        threading/main-thread.cpp
        threading/thread.cpp
        threading/thread_private.cpp
        threading/thread_private.hpp
        time/clock.cpp
        time/delta.cpp
        time/realtime-clock.cpp
        time/timer.cpp
)

if (BUILD_SHARED_LIBRARY)
    add_library(wiztk-system SHARED ${config_header} ${system_sources})
    set_target_properties(wiztk-system PROPERTIES VERSION 1 SOVERSION 1)
else ()
    add_library(wiztk-system ${config_header} ${system_sources})
endif ()

target_link_libraries(
        wiztk-system
        PUBLIC rt
        PUBLIC ${PTHREAD_LIBRARY}
        PUBLIC ${SYSTEMD_LIBRARY}
        PUBLIC wiztk-base
)

install(TARGETS wiztk-system DESTINATION lib)
