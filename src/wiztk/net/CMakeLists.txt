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
        net_sources
        ${PROJECT_SOURCE_DIR}/include/wiztk/net/address-family.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/net/address-info.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/net/io-buffer.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/net/ip-address.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/net/protocol-type.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/net/server-socket.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/net/socket.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/net/socket-exception.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/net/socket-type.hpp
        address-info/private.hpp
        address-info.cpp
        io-buffer.cpp
        ip-address/native.cpp
        ip-address/native.hpp
        ip-address/private.cpp
        ip-address/private.hpp
        ip-address.cpp
        server-socket.cpp
        socket.cpp
)

if (BUILD_SHARED_LIBRARY)
    add_library(wiztk-net SHARED ${config_header} ${net_sources})
    set_target_properties(wiztk-net PROPERTIES VERSION 1 SOVERSION 1)
else ()
    add_library(wiztk-net ${config_header} ${net_sources})
endif ()

target_link_libraries(
        wiztk-net
        PUBLIC wiztk-base
)

install(TARGETS wiztk-net DESTINATION lib)
