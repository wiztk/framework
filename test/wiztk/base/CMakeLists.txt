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

# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# core module
add_subdirectory(delegate)
add_subdirectory(sigcxx)
add_subdirectory(margin)
add_subdirectory(rect)
add_subdirectory(color)
add_subdirectory(memory)
add_subdirectory(object)
add_subdirectory(vectors)
add_subdirectory(deque)
add_subdirectory(compound-deque)
add_subdirectory(trace)
add_subdirectory(posix-timer)
