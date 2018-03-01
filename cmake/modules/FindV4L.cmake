#
# - Find the V4L include file and library
#
#  V4L_FOUND - system has V4L
#  V4L_INCLUDE_DIRS - the V4L include directory
#  V4L_LIBRARIES - The libraries needed to use V4L

find_package(PkgConfig)

pkg_check_modules(V4L_PKG QUIET libv4l2)

find_path(
        V4L_INCLUDE_DIR
        NAMES libv4l2.h
        PATHS /usr/local/include /usr/include
        HINTS ${V4L_PKG_INCLUDE_DIRS}
)

find_library(
        V4L_LIBRARY
        NAMES v4l2 ${V4L_PKG_LIBRARY}
        PATHS /usr/local /usr
        HINTS ${V4L_PKG_LIBRARY_DIRS}
        PATH_SUFFIXES lib64 lib
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(V4L DEFAULT_MSG V4L_LIBRARY V4L_INCLUDE_DIR )

mark_as_advanced(V4L_INCLUDE_DIR V4L_LIBRARY)
