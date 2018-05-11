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
        graphics_sources
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/abstract-surface.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/bitmap.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/canvas.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/clip-operation.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/font.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/font-style.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/gradient-shader.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/image-info.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/matrix.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/paint.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/path.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/shader.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/surface.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/text-box.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/typeface.hpp
        abstract-surface.cpp
        bitmap.cpp
        bitmap_private.hpp
        canvas.cpp
        canvas_private.hpp
        font.cpp
        font-style.cpp
        font-style_private.hpp
        font_private.hpp
        gradient-shader.cpp
        image-info.cpp
        image-info_private.hpp
        matrix.cpp
        matrix_private.hpp
        paint.cpp
        path.cpp
        shader.cpp
        shader_private.hpp
        surface_private.hpp
        text-box.cpp
        typeface.cpp
        typeface_private.hpp
        view-surface.cpp
)

if (BUILD_SHARED_LIBRARY)
    add_library(wiztk-graphics SHARED ${config_header} ${graphics_sources})
    set_target_properties(wiztk-graphics PROPERTIES VERSION 1 SOVERSION 1)
else ()
    add_library(wiztk-graphics ${config_header} ${graphics_sources})
endif ()

target_link_libraries(
        wiztk-graphics
        PUBLIC wiztk-base
        PUBLIC ${OPENGL_LIBRARIES}
        PUBLIC ${PNG_LIBRARIES}
        PUBLIC ${JPEG_LIBRARIES}
        PUBLIC ${OPENIMAGEIO_LIBRARY}
        PUBLIC ${FREETYPE_LIBRARIES}
        PUBLIC ${FONTCONFIG_LIBRARY}
        PUBLIC ${ICU_LIBRARIES}
        PUBLIC ${WEBP_LIBRARY}
        PUBLIC ${WEBPMUX_LIBRARY}
        PUBLIC ${WEBPDEMUX_LIBRARY}
        PUBLIC ${DL_LIBRARY}
        PUBLIC ${PTHREAD_LIBRARY}
#        PUBLIC ${LIBS}
        PRIVATE skia
)

install(TARGETS wiztk-graphics DESTINATION lib)
