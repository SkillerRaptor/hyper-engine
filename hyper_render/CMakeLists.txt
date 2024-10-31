#-------------------------------------------------------------------------------------------
# Copyright (c) 2024, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------
set(SOURCES
        src/hyper_render/camera.cpp
        src/hyper_render/renderer.cpp)

set(HEADERS
        include/hyper_render/camera.hpp
        include/hyper_render/renderer.hpp)

hyperengine_define_library(hyper_render)
target_include_directories(
        hyper_render
        PRIVATE
        ${CMAKE_SOURCE_DIR}/assets/shaders)
target_link_libraries(
        hyper_render
        PUBLIC
        hyper_core
        hyper_event
        hyper_rhi
        hyper_platform
        fastgltf
        glm
        meshoptimizer)

