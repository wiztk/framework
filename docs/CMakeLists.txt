# Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
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

# ----------------------------------------------------------------------------
# Document build option
# ----------------------------------------------------------------------------
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if (BUILD_DOCUMENTATION)

    # Use doxygen:
    if (NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif ()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile.in)
    set(doxyfile ${PROJECT_BINARY_DIR}/Doxyfile)
    set(doxy_main_page ${PROJECT_SOURCE_DIR}/README.md)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
            COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
    #    add_custom_command(TARGET doc
    #            POST_BUILD
    #            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/docs ${CMAKE_CURRENT_BINARY_DIR}/html/docs
    #            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    #            COMMENT "Copy \"${CMAKE_CURRENT_SOURCE_DIR}/doc\" to \"${CMAKE_CURRENT_BINARY_DIR}/html\""
    #            VERBATIM)

    # install(DIRECTORY ${PROJECT_BINARY_DIR}/html DESTINATION share/doc)

    # Use latex:
    find_package(LATEX)
    if (LATEX_FOUND)
        if (UNIX)
            set(COPY_OR_LINK create_symlink)
            set(COPY_DIRECTORY_OR_LINK create_symlink)
            set(COPY_OR_LINK_MESSAGE "Create symlink of")
            set(COPY_DIRECTORY_OR_LINK_MESSAGE "Create symlink of")
        else ()
            set(COPY_OR_LINK copy)
            set(COPY_DIRECTORY_OR_LINK copy_directory)
            set(COPY_OR_LINK_MESSAGE "Copy")
            set(COPY_DIRECTORY_OR_LINK_MESSAGE "Copy directory of")
        endif ()

        add_subdirectory(latex)
        add_dependencies(doc tikz)
    endif ()

endif ()

