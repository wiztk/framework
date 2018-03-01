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

cmake_minimum_required(VERSION 3.1)

project(WizTK)

# The project version number.
set(VERSION_MAJOR 0 CACHE STRING "Project major version number.")
set(VERSION_MINOR 0 CACHE STRING "Project minor version number.")
set(VERSION_PATCH 1 CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules/")
include(cmake/functions.cmake)

option(BUILD_UNIT_TEST "Build unit test code" OFF)
option(BUILD_SHARED_LIBRARY "Build shared library" OFF)
option(TRACE "Turn trace mode on/off" ON)   # Turn on in development stage
option(VERBOSE "Turn verbose mode on/off" OFF)

# ----------------------------------------------------------------------------
# System check
# ----------------------------------------------------------------------------

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(APPLE TRUE)
    set(UNIX TRUE)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LINUX TRUE)
    set(UNIX TRUE)
    add_definitions(-D__linux__)
else ()
    message(FATAL_ERROR "Unsupported system!")
endif ()

# ----------------------------------------------------------------------------
# Compiler and basic compile options
# ----------------------------------------------------------------------------

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # using clang
    message(STATUS "Use C++ compiler: Clang")
    # add_definitions(-D__clang__)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
    message(STATUS "Use C++ compiler: GCC")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    # using Intel C++
    message(STATUS "Use C++ compiler: Intel C++")
else ()
    message(FATAL "Unsupported c++ compiler: ${CMAKE_CXX_COMPILER}")
endif ()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()
set(CMAKE_CXX_STANDARD 14)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D__DEBUG__)
    set(CMAKE_CXX_FLAGS "-g -Wall -std=c++14")
else ()
    set(CMAKE_CXX_FLAGS "-O3 -std=c++14")
endif ()

if (TRACE)
    add_definitions(-D__TRACE__)
endif ()

# ----------------------------------------------------------------------------
# Find prerequisites
# ----------------------------------------------------------------------------

find_package(EGL)
if (EGL_FOUND)
    # set(HAVE_EGL 1)
    include_directories(${EGL_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${EGL_LIBRARIES})
endif ()

find_package(FontConfig REQUIRED)
if (FONTCONFIG_FOUND)
    include_directories(${FONTCONFIG_INCLUDE_DIR})
    set(LIBS ${LIBS} ${FONTCONFIG_LIBRARY})
endif ()

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
    set(LIBS ${LIBS} ${OPENGL_LIBRARIES})
endif ()

find_package(GLES2 REQUIRED)
if (GLES2_FOUND)
    include_directories(${GLES2_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${GLES2_LIBRARIES})
endif ()

find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    include_directories(Vulkan_INCLUDE_DIRS)
    set(LIBS ${LIBS} ${Vulkan_LIBRARIES})
    add_definitions(-DVK_USE_PLATFORM_WAYLAND_KHR)
endif ()

find_package(Wayland REQUIRED)
if (WAYLAND_FOUND)
    include_directories(${WAYLAND_CLIENT_INCLUDE_DIR} ${WAYLAND_EGL_INCLUDE_DIR})
    set(LIBS ${LIBS} ${WAYLAND_CLIENT_LIBRARIES} ${WAYLAND_EGL_LIBRARIES})
endif ()
#add_definitions(-D__UNIX__)

find_package(XKBCommon REQUIRED)
if (XKBCommon_FOUND)
    include_directories(${XKBCOMMON_INCLUDE_DIRS})
    add_definitions(${XKBCOMMON_DEFINITIONS})
    set(LIBS ${LIBS} ${XKBCOMMON_LIBRARIES})
endif ()

find_package(LibInput REQUIRED)
if (LIBINPUT_FOUND)
    include_directories(${LIBINPUT_INCLUDE_DIRS})
    add_definitions(${LIBINPUT_DEFINITIONS})
    set(LIBS ${LIBS} ${LIBINPUT_LIBRARIES})
endif ()

find_package(PNG REQUIRED)
if (PNG_FOUND)
    include_directories(${PNG_INCLUDE_DIRS})
    add_definitions(${PNG_DEFINITIONS})
    set(LIBS ${LIBS} ${PNG_LIBRARIES})
endif ()

find_package(JPEG REQUIRED)
if (JPEG_FOUND)
    include_directories(${JPEG_INCLUDE_DIRS})
    add_definitions(${JPEG_DEFINITIONS})
    set(LIBS ${LIBS} ${JPEG_LIBRARIES})
endif ()

#find_package(FFMPEG REQUIRED)
#if (FFMPEG_FOUND)
#    message()
#    include_directories(${FFMPEG_INCLUDE_DIRS})
#    add_definitions(${FFMPEG_DEFINITIONS})
#    set(LIBS ${LIBS} ${FFMPEG_LIBRARIES})
#endif ()

find_package(OpenImageIO REQUIRED)
if (OPENIMAGEIO_FOUND)
    include_directories(${OPENIMAGEIO_INCLUDE_DIR})
    set(LIBS ${LIBS} ${OPENIMAGEIO_LIBRARY})
endif ()

find_package(Freetype REQUIRED)
if (FREETYPE_FOUND)
    include_directories(${FREETYPE_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${FREETYPE_LIBRARIES})
endif ()

find_package(FontConfig REQUIRED)
if (FONTCONFIG_FOUND)
    include_directories(${FONTCONFIG_INCLUDE_DIR})
    set(LIBS ${LIBS} ${FONTCONFIG_LIBRARY})
endif ()

find_package(ICU COMPONENTS uc i18n io REQUIRED)
if (ICU_FOUND)
    include_directories(${ICU_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${ICU_LIBRARIES})
endif ()

find_package(V4L REQUIRED)
if (V4L_FOUND)
    include_directories(${V4L_INCLUDE_DIR})
    set(LIBS ${LIBS} ${V4L_LIBRARY})
endif ()

find_library(DL_LIBRARY "dl")
set(LIBS ${LIBS} ${DL_LIBRARY})

find_library(PTHREAD_LIBRARY "pthread")
set(LIBS ${LIBS} ${PTHREAD_LIBRARY})

find_library(WEBP_LIBRARY "webp")
set(LIBS ${LIBS} ${WEBP_LIBRARY})

find_library(WEBPMUX_LIBRARY "webpmux")
set(LIBS ${LIBS} ${WEBPMUX_LIBRARY})

find_library(WEBPDEMUX_LIBRARY "webpdemux")
set(LIBS ${LIBS} ${WEBPDEMUX_LIBRARY})

# Optional packages:

find_package(Systemd)
if (SYSTEMD_FOUND)
    set(HAVE_SYSTEMD 1)
    include_directories(${SYSTEMD_INCLUDE_DIR})
    set(LIBS ${LIBS} ${SYSTEMD_LIBRARY})
endif ()

# ----------------------------------------------------------------------------
# Global definitions
# ----------------------------------------------------------------------------

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include)

if (BUILD_SHARED_LIBRARY)
    add_definitions(-DSKLAND_SHARED_EXPORT)
endif ()

# Prebuilts setting:

set(PREBUILTS_DIR ${PROJECT_BINARY_DIR}/prebuilts/${CMAKE_SYSTEM_PROCESSOR})

# Skia:
set(SKIA_INCLUDE_DIR ${PREBUILTS_DIR}/skia/include)
set(SKIA_LIB_DIR ${PREBUILTS_DIR}/skia/lib)
list(APPEND SKIA_PUBLIC_INCLUDES
        "${SKIA_INCLUDE_DIR}/android"
        "${SKIA_INCLUDE_DIR}/c"
        "${SKIA_INCLUDE_DIR}/codec"
        "${SKIA_INCLUDE_DIR}/config"
        "${SKIA_INCLUDE_DIR}/core"
        "${SKIA_INCLUDE_DIR}/effects"
        "${SKIA_INCLUDE_DIR}/gpu"
        "${SKIA_INCLUDE_DIR}/gpu/effects"
        "${SKIA_INCLUDE_DIR}/gpu/gl"
        "${SKIA_INCLUDE_DIR}/images"
        "${SKIA_INCLUDE_DIR}/pathops"
        "${SKIA_INCLUDE_DIR}/ports"
        "${SKIA_INCLUDE_DIR}/utils"
        "${SKIA_INCLUDE_DIR}/private")
include_directories(${SKIA_PUBLIC_INCLUDES})
link_directories(${SKIA_LIB_DIR})

# Wayland protocols
include_directories(${PREBUILTS_DIR}/wayland-protocols/unstable/xdg-shell)
link_directories(${PREBUILTS_DIR}/wayland-protocols/unstable/xdg-shell)

# Add this difinition to change the ordering to work in Wayland/X windows.
add_definitions(-DSK_SAMPLES_FOR_X)

# ----------------------------------------------------------------------------
# Sub directories
# ----------------------------------------------------------------------------

add_subdirectory(docs)
#add_subdirectory(deps)
#add_subdirectory(third_party)
add_subdirectory(src)

if (LINUX)
    add_subdirectory(examples)
endif ()

if (BUILD_UNIT_TEST)

    find_package(GTest REQUIRED)

    if (GTEST_FOUND)
        add_definitions(-D__TEST__)
        include_directories(${GTEST_INCLUDE_DIRS})
        add_subdirectory(test)
    endif ()

endif ()

# ----------------------------------------------------------------------------
# Generate config.hpp
# ----------------------------------------------------------------------------
configure_file(
        "${PROJECT_SOURCE_DIR}/include/wiztk/config.hpp.in"
        "${PROJECT_BINARY_DIR}/include/wiztk/config.hpp" @ONLY
)

# ----------------------------------------------------------------------------
# Installation:
# ----------------------------------------------------------------------------

install(
        DIRECTORY "${PROJECT_SOURCE_DIR}/include/wiztk"
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp"
)
install(
        FILES "${PROJECT_BINARY_DIR}/include/wiztk/config.hpp"
        DESTINATION include/wiztk
)

# ----------------------------------------------------------------------------
# Summary:
# ----------------------------------------------------------------------------

message(STATUS "Install path: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Use Wayland client library: ${WAYLAND_CLIENT_LIBRARIES}")
if (EGL_FOUND)
    message(STATUS "Use EGL library: ${EGL_LIBRARIES}")
endif ()

message(STATUS "IMPORTANT: checkout 'https://github.com/wiztk/prebuilts.git' to ${PROJECT_BINARY_DIR}/prebuilts.")
