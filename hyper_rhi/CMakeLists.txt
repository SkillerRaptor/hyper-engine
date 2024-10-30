#-------------------------------------------------------------------------------------------
# Copyright (c) 2024, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------
set(SOURCES
        src/hyper_rhi/buffer.cpp
        src/hyper_rhi/compute_pipeline.cpp
        src/hyper_rhi/graphics_device.cpp
        src/hyper_rhi/graphics_pipeline.cpp
        src/hyper_rhi/pipeline_layout.cpp
        src/hyper_rhi/queue.cpp
        src/hyper_rhi/render_pass.cpp
        src/hyper_rhi/resource.cpp
        src/hyper_rhi/resource_handle.cpp
        src/hyper_rhi/shader_compiler.cpp
        src/hyper_rhi/shader_module.cpp
        src/hyper_rhi/surface.cpp
        src/hyper_rhi/texture.cpp
        src/hyper_rhi/vulkan/vulkan_buffer.cpp
        src/hyper_rhi/vulkan/vulkan_command_list.cpp
        src/hyper_rhi/vulkan/vulkan_descriptor_manager.cpp
        src/hyper_rhi/vulkan/vulkan_graphics_device.cpp
        src/hyper_rhi/vulkan/vulkan_graphics_pipeline.cpp
        src/hyper_rhi/vulkan/vulkan_pipeline_layout.cpp
        src/hyper_rhi/vulkan/vulkan_queue.cpp
        src/hyper_rhi/vulkan/vulkan_render_pass.cpp
        src/hyper_rhi/vulkan/vulkan_shader_module.cpp
        src/hyper_rhi/vulkan/vulkan_surface.cpp
        src/hyper_rhi/vulkan/vulkan_texture.cpp
        src/hyper_rhi/vulkan/vulkan_upload_manager.cpp)

set(HEADERS
        include/hyper_rhi/buffer.hpp
        include/hyper_rhi/command_list.hpp
        include/hyper_rhi/compute_pipeline.hpp
        include/hyper_rhi/graphics_device.hpp
        include/hyper_rhi/graphics_pipeline.hpp
        include/hyper_rhi/pipeline_layout.hpp
        include/hyper_rhi/queue.hpp
        include/hyper_rhi/render_pass.hpp
        include/hyper_rhi/resource.hpp
        include/hyper_rhi/resource_handle.hpp
        include/hyper_rhi/shader_module.hpp
        include/hyper_rhi/shader_compiler.hpp
        include/hyper_rhi/surface.hpp
        include/hyper_rhi/texture.hpp
        include/hyper_rhi/vulkan/vulkan_buffer.hpp
        include/hyper_rhi/vulkan/vulkan_command_list.hpp
        include/hyper_rhi/vulkan/vulkan_common.hpp
        include/hyper_rhi/vulkan/vulkan_descriptor_manager.hpp
        include/hyper_rhi/vulkan/vulkan_graphics_device.hpp
        include/hyper_rhi/vulkan/vulkan_graphics_pipeline.hpp
        include/hyper_rhi/vulkan/vulkan_pipeline_layout.hpp
        include/hyper_rhi/vulkan/vulkan_queue.hpp
        include/hyper_rhi/vulkan/vulkan_render_pass.hpp
        include/hyper_rhi/vulkan/vulkan_shader_module.hpp
        include/hyper_rhi/vulkan/vulkan_surface.hpp
        include/hyper_rhi/vulkan/vulkan_texture.hpp
        include/hyper_rhi/vulkan/vulkan_upload_manager.hpp)

if (WIN32)
    set(SOURCES
            ${SOURCES}
            src/hyper_rhi/d3d12/d3d12_graphics_device.cpp
            src/hyper_rhi/d3d12/d3d12_surface.cpp)

    set(HEADERS
            ${HEADERS}
            include/hyper_rhi/d3d12/d3d12_common.hpp
            include/hyper_rhi/d3d12/d3d12_graphics_device.hpp
            include/hyper_rhi/d3d12/d3d12_surface.hpp)
endif ()

hyperengine_define_library(hyper_rhi)
target_link_libraries(
        hyper_rhi
        PUBLIC
        hyper_core
        hyper_platform
        DirectXShaderCompiler
        fmt
        glfw
        volk
        VulkanMemoryAllocator)

if (WIN32)
    target_link_libraries(
            hyper_rhi
            PUBLIC
            d3d12.lib
            dxgi.lib
            D3D12MemoryAllocator)
endif ()
