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
        device_sources
        ${PROJECT_SOURCE_DIR}/include/wiztk/device/video/camera.hpp
        video/camera.cpp
)

if (BUILD_SHARED_LIBRARY)
    add_library(wiztk-device SHARED ${config_header} ${device_sources})
    set_target_properties(wiztk-device PROPERTIES VERSION 1 SOVERSION 1)
else ()
    add_library(wiztk-device ${config_header} ${device_sources})
endif ()

#target_link_libraries(
#        wiztk-base
#        PUBLIC rt
#        PUBLIC ${LIBS}
#        PUBLIC wayland-cursor
#        PRIVATE skia
#        PRIVATE xdg-shell-unstable-v6
#        #        PRIVATE icu_utf # in third_party
#)

install(TARGETS wiztk-device DESTINATION lib)
