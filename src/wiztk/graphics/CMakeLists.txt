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

set(graphics_sources
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/alignment.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/text-alignment.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/bitmap.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/canvas.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/color-space.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/clip-operation.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/font.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/font-style.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/gradient-shader.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/image.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/image-info.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/matrix.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/paint.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/path.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/pixmap.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/shader.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/surface.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/surface-props.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/graphics/typeface.hpp
        bitmap/private.hpp
        bitmap.cpp
        canvas/native.hpp
        canvas/private.hpp
        canvas.cpp
        color-space/private.hpp
        color-space.cpp
        font/private.hpp
        font.cpp
        font-style/private.hpp
        font-style.cpp
        gradient-shader.cpp
        image/private.hpp
        image.cpp
        image-info/private.hpp
        image-info.cpp
        matrix/private.hpp
        matrix.cpp
        paint.cpp
        path.cpp
        pixmap.cpp
        shader/private.hpp
        shader.cpp
        surface/private.hpp
        surface.cpp
        surface-props/private.hpp
        surface-props.cpp
        typeface/private.hpp
        typeface.cpp
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
