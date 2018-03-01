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

#file(GLOB_RECURSE base_headers "${PROJECT_SOURCE_DIR}/include/wiztk/base/*.hpp")
#file(GLOB_RECURSE base_sources "base/*.cpp")
#list(APPEND all_source_files ${base_headers} ${base_sources})
#
#file(GLOB_RECURSE numerical_headers "${PROJECT_SOURCE_DIR}/include/wiztk/numerical/*.hpp")
#file(GLOB_RECURSE numerical_sources "numerical/*.cpp")
#list(APPEND all_source_files ${numerical_headers} ${numerical_sources})
#
#file(GLOB_RECURSE net_headers "${PROJECT_SOURCE_DIR}/include/wiztk/net/*.hpp")
#file(GLOB_RECURSE net_sources "net/*.cpp")
#file(GLOB_RECURSE net_internal_headers "net/*.hpp")
#list(APPEND all_source_files ${net_headers} ${net_sources} ${net_internal_headers})
#
#file(GLOB_RECURSE graphic_headers "${PROJECT_SOURCE_DIR}/include/wiztk/graphic/*.hpp")
#file(GLOB_RECURSE graphic_sources "graphic/*.cpp")
#list(APPEND all_source_files ${graphic_headers} ${graphic_sources})
#
#file(GLOB_RECURSE gui_headers "${PROJECT_SOURCE_DIR}/include/wiztk/gui/*.hpp")
#file(GLOB_RECURSE gui_sources "gui/*.cpp")
#list(APPEND all_source_files ${gui_headers} ${gui_sources})

#file(GLOB_RECURSE public_headers "${PROJECT_SOURCE_DIR}/include/wiztk/*.hpp")
#file(GLOB_RECURSE internal_headers "./*.hpp")
#file(GLOB_RECURSE sources "./*.cpp")
#list(APPEND all_source_files ${public_headers} ${internal_headers} ${sources})

set(config_header "${PROJECT_BINARY_DIR}/include/wiztk/config.hpp")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#if (BUILD_SHARED_LIBRARY)
#    add_library(wiztk SHARED ${config_header} ${all_source_files})
#    set_target_properties(wiztk PROPERTIES VERSION 1 SOVERSION 1)
#else ()
#    add_library(wiztk ${config_header} ${all_source_files})
#endif ()
#
#target_include_directories(
#        wiztk
#        PRIVATE "${PROJECT_SOURCE_DIR}/include/wiztk"
#        #        PRIVATE "${PROJECT_SOURCE_DIR}/third_party/icu"
#)
#
##    target_link_libraries(
##            wiztk
##            rt
##            ${LIBS}
##            wayland-cursor
##            skia
##            xdg-shell-unstable-v6-client
##            OpenImageIO
##            OpenImageIO_Util
##            chromium_base
##    )
#target_link_libraries(
#        wiztk
#        PUBLIC rt
#        PUBLIC ${LIBS}
#        PUBLIC wayland-cursor
#        PRIVATE skia
#        PRIVATE xdg-shell-unstable-v6
#        #        PRIVATE icu_utf # in third_party
#)

#install(TARGETS wiztk DESTINATION lib)

add_subdirectory(base)
add_subdirectory(async)
add_subdirectory(system)
add_subdirectory(device)
add_subdirectory(graphic)
add_subdirectory(net)
add_subdirectory(gui)