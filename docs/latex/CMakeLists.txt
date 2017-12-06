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

add_custom_target(
        tikz
#        ALL
        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/docs/latex"
)

# -----------------------------------------------------------------------------
# signal_slot:

set(LATEX_SOURCE_DIR "${PROJECT_SOURCE_DIR}/docs/latex")
set(LATEX_BINARY_DIR "${PROJECT_BINARY_DIR}/docs/latex")

add_custom_command(
        TARGET tikz
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E ${COPY_OR_LINK} "${LATEX_SOURCE_DIR}/signal_slot.tex" "${LATEX_BINARY_DIR}/signal_slot.tex"
        WORKING_DIRECTORY ${LATEX_BINARY_DIR}
        COMMENT "${COPY_OR_LINK_MESSAGE} \"${LATEX_SOURCE_DIR}/signal_slot.tex\" to \"${LATEX_BINARY_DIR}/signal_slot.tex\""
        VERBATIM)

add_custom_command(
        TARGET tikz
        COMMAND "${PDFLATEX_COMPILER}"
        ARGS "-shell-escape" "signal_slot" "1>/dev/null" "2>&1"
        DEPENDS "${LATEX_BINARY_DIR}/signal_slot.tex"
        WORKING_DIRECTORY "${LATEX_BINARY_DIR}"
        COMMENT "${PDFLATEX_COMPILER} ${LATEX_BINARY_DIR}/signal_slot.tex")

add_custom_command(
        TARGET tikz
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E ${COPY_OR_LINK} "${LATEX_SOURCE_DIR}/main_loop.tex" "${LATEX_BINARY_DIR}/main_loop.tex"
        WORKING_DIRECTORY ${LATEX_BINARY_DIR}
        COMMENT "${COPY_OR_LINK_MESSAGE} \"${LATEX_SOURCE_DIR}/main_loop.tex\" to \"${LATEX_BINARY_DIR}/main_loop.tex\""
        VERBATIM)

add_custom_command(
        TARGET tikz
        COMMAND "${PDFLATEX_COMPILER}"
        ARGS "-shell-escape" "main_loop" "1>/dev/null" "2>&1"
        DEPENDS "${LATEX_BINARY_DIR}/main_loop.tex"
        WORKING_DIRECTORY "${LATEX_BINARY_DIR}"
        COMMENT "${PDFLATEX_COMPILER} ${LATEX_BINARY_DIR}/main_loop.tex")
