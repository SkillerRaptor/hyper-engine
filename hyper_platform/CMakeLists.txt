#-------------------------------------------------------------------------------------------
# Copyright (c) 2024-present, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------
set(SOURCES
        src/hyper_platform/hyper_platform.cpp
        src/hyper_platform/input.cpp
        src/hyper_platform/key_events.cpp
        src/hyper_platform/mouse_events.cpp
        src/hyper_platform/sdl_event.cpp
        src/hyper_platform/window.cpp
        src/hyper_platform/window_events.cpp)

set(HEADERS
        include/hyper_platform/forward.hpp
        include/hyper_platform/hyper_platform.hpp
        include/hyper_platform/input.hpp
        include/hyper_platform/key_codes.hpp
        include/hyper_platform/key_events.hpp
        include/hyper_platform/mouse_codes.hpp
        include/hyper_platform/mouse_events.hpp
        include/hyper_platform/sdl_event.hpp
        include/hyper_platform/window.hpp
        include/hyper_platform/window_events.hpp)

hyperengine_define_library(hyper_platform)
target_link_libraries(
        hyper_platform
        PUBLIC
        hyper_core
        hyper_event
        glm
        SDL3::SDL3)

