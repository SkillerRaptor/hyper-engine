#-------------------------------------------------------------------------------------------
# Copyright (c) 2024, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------
set(SOURCES
        src/hyper_render/camera.cpp
        src/hyper_render/material.cpp
        src/hyper_render/mesh.cpp
        src/hyper_render/renderable.cpp
        src/hyper_render/renderer.cpp
        src/hyper_render/render_passes/grid_pass.cpp
        src/hyper_render/render_passes/imgui_pass.cpp
        src/hyper_render/render_passes/opaque_pass.cpp)

set(HEADERS
        include/hyper_render/camera.hpp
        include/hyper_render/material.hpp
        include/hyper_render/mesh.hpp
        include/hyper_render/renderable.hpp
        include/hyper_render/renderer.hpp
        include/hyper_render/render_passes/grid_pass.hpp
        include/hyper_render/render_passes/imgui_pass.hpp
        include/hyper_render/render_passes/opaque_pass.hpp)

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
        imgui
        meshoptimizer
        stb)

