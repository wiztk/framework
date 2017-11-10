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

# Set global variable
# 
# Usage:
#   global_set(<variable> [arg1 arg2 ...])
#
function(global_set name)
    set_property(GLOBAL PROPERTY "${name}_property" ${ARGN})
endfunction()

# Get global variable
# 
# Usage:
#   global_get(<variable>)
#
function(global_get name)
    get_property(temp GLOBAL PROPERTY "${name}_property")
    set(${name} ${temp} PARENT_SCOPE)
endfunction()

# A simple function to add files into a given list
function(add_file list_name option)
    file(${option} files_to_add ${ARGN})
    list(APPEND ${list_name} ${files_to_add})
    set(${list_name} ${${list_name}} PARENT_SCOPE)
endfunction()

# A simple function to remove files from a given file list
#
# Params:
#   - list_name: the file list
#   - option: the options passed to cmake 'file' command, must be GLOB or GLOB_RECURSE
#   - ARGN: the files need to be removed
function(remove_file list_name option)
    file(${option} files_to_remove ${ARGN})
    list(REMOVE_ITEM ${list_name} ${files_to_remove})
    set(${list_name} ${${list_name}} PARENT_SCOPE)
endfunction()
