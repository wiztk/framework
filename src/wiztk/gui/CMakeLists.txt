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
        gui_sources
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/abstract-button.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/abstract-egl-backend.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/abstract-epoll-task.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/abstract-event-handler.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/abstract-layout.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/abstract-rendering-api.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/abstract-shell-view.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/abstract-slider.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/abstract-view.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/anchor.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/anchor-group.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/application.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/buffer.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/callback.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/context.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/cursor.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/dialog.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/display.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/gl-view.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/gl-window.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/glesv2-api.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/gles2-backend.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/input.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/input-event.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/input-manager.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/key-event.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/label.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/linear-layout.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/main-loop.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/main-window.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/mouse-event.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/output.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/output-manager.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/push-button.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/queued-task.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/region.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/relative-layout.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/shared-memory-pool.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/slider.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/spinner.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/theme.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/timer.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/title-bar.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/tooltip.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/touch-event.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/video-view.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/view-surface.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/vulkan-api.hpp
        ${PROJECT_SOURCE_DIR}/include/wiztk/gui/window.hpp
        abstract-button.cpp
        abstract-egl-backend/private.hpp
        abstract-egl-backend.cpp
        abstract-event-handler.cpp
        abstract-event-handler/private.hpp
        abstract-layout.cpp
        abstract-rendering-api.cpp
        abstract-rendering-api_private.hpp
        abstract-rendering-api_proxy.hpp
        abstract-shell-view/private.cpp
        abstract-shell-view/private.hpp
        abstract-shell-view.cpp
        abstract-view/iterators.hpp
        abstract-view/private.hpp
        abstract-view.cpp
        anchor.cpp
        anchor-group.cpp
        application.cpp
        buffer/private.cpp
        buffer/private.hpp
        buffer.cpp
        callback.cpp
        cursor.cpp
        dialog.cpp
        display/private.cpp
        display/private.hpp
        display.cpp
        gl-view.cpp
        gl-window.cpp
        glesv2-api.cpp
        gles2-backend/private.hpp
        gles2-backend/private.cpp
        gles2-backend.cpp
        input/private.cpp
        input/private.hpp
        input.cpp
        input-event.cpp
        input-manager.cpp
        keyboard_state.cpp
        keyboard_state.hpp
        keymap.cpp
        keymap.hpp
        label.cpp
        linear-layout.cpp
        main-loop/display-event.cpp
        main-loop/display-event.hpp
        main-loop/private.cpp
        main-loop/private.hpp
        main-loop/signal-event.cpp
        main-loop/signal-event.hpp
        main-loop.cpp
        main-window.cpp
        mouse-event/private.hpp
        mouse-event.cpp
        output/private.cpp
        output/private.hpp
        output.cpp
        output-manager.cpp
        push-button.cpp
        region.cpp
        relative-layout.cpp
        shared-memory-pool.cpp
        slider.cpp
        spinner.cpp
        theme.cpp
        theme-dark.cpp
        theme-dark.hpp
        theme-light.cpp
        theme-light.hpp
        timer.cpp
        title-bar.cpp
        tooltip.cpp
        video-view.cpp
        view-surface/shell/popup/private.hpp
        view-surface/shell/toplevel/private.cpp
        view-surface/shell/toplevel/private.hpp
        view-surface/shell/private.cpp
        view-surface/shell/private.hpp
        view-surface/private.cpp
        view-surface/private.hpp
        view-surface.cpp
        vulkan-api.cpp
        window.cpp
)

if (BUILD_SHARED_LIBRARY)
    add_library(wiztk-gui SHARED ${config_header} ${gui_sources})
    set_target_properties(wiztk-gui PROPERTIES VERSION 1 SOVERSION 1)
else ()
    add_library(wiztk-gui ${config_header} ${gui_sources})
endif ()

target_link_libraries(
        wiztk-gui
        PUBLIC wiztk-graphics
        PUBLIC wiztk-async
        PUBLIC ${WAYLAND_CLIENT_LIBRARIES}
        PUBLIC ${WAYLAND_EGL_LIBRARIES}
        #        PUBLIC rt
        #        PUBLIC ${LIBS}
        PUBLIC wayland-cursor
        PUBLIC ${XKBCOMMON_LIBRARIES}
        PUBLIC ${LIBINPUT_LIBRARIES}
        PUBLIC ${EGL_LIBRARIES}
        PUBLIC ${Vulkan_LIBRARIES}
        PUBLIC ${V4L_LIBRARY}
        #        PRIVATE skia
        PRIVATE xdg-shell-unstable-v6
        #        PRIVATE icu_utf # in third_party
)

target_include_directories(
        wiztk-gui
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS wiztk-gui DESTINATION lib)
