#-------------------------------------------------------------------------------------------
# Copyright (c) 2024, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------
set(SOURCES
        src/hyper_platform/input.cpp
        src/hyper_platform/window.cpp)

set(HEADERS
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

