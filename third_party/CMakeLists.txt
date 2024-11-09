#-------------------------------------------------------------------------------------------
# Copyright (c) 2024, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------

include(FetchContent)

#-------------------------------------------------------------------------------------------
# argparse
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        argparse
        SYSTEM
        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
        GIT_TAG v3.1)

set(ARGPARSE_INSTALL OFF CACHE INTERNAL "")
set(ARGPARSE_BUILD_TESTS OFF CACHE INTERNAL "")
set(ARGPARSE_BUILD_SAMPLES OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(argparse)

set_target_properties(
        argparse
        PROPERTIES
        FOLDER "third_party")

#-------------------------------------------------------------------------------------------
# D3D12MemoryAllocator
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        D3D12MemoryAllocator
        SYSTEM
        GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/D3D12MemoryAllocator.git
        GIT_TAG master)

set(D3D12MA_BUILD_SAMPLE OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(D3D12MemoryAllocator)

set_target_properties(
        D3D12MemoryAllocator
        PROPERTIES
        FOLDER "third_party")

#-------------------------------------------------------------------------------------------
# DirectXShaderCompiler
#-------------------------------------------------------------------------------------------
hyperengine_download_and_extract(
        https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.8.2407/dxc_2024_07_31.zip
        ${CMAKE_SOURCE_DIR}/third_party
        DirectXShaderCompiler)

add_library(DirectXShaderCompiler INTERFACE)
target_link_libraries(
        DirectXShaderCompiler
        INTERFACE
        ${CMAKE_SOURCE_DIR}/third_party/DirectXShaderCompiler/lib/x64/dxcompiler.lib)
target_include_directories(
        DirectXShaderCompiler
        SYSTEM
        INTERFACE
        ${CMAKE_SOURCE_DIR}/third_party/DirectXShaderCompiler/inc)

set_target_properties(
        DirectXShaderCompiler
        PROPERTIES
        FOLDER "third_party")

hyperengine_deploy_files(${CMAKE_CURRENT_LIST_DIR}/DirectXShaderCompiler/bin/x64/ hyper_engine/)

#-------------------------------------------------------------------------------------------
# fastgltf
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        fastgltf
        SYSTEM
        GIT_REPOSITORY https://github.com/spnda/fastgltf.git
        GIT_TAG v0.8.0)

set(FASTGLTF_USE_CUSTOM_SMALLVECTOR ON CACHE INTERNAL "")
set(FASTGLTF_ENABLE_TESTS OFF CACHE INTERNAL "")
set(FASTGLTF_ENABLE_EXAMPLES OFF CACHE INTERNAL "")
set(FASTGLTF_ENABLE_DOCS OFF CACHE INTERNAL "")
set(FASTGLTF_ENABLE_GLTF_RS OFF CACHE INTERNAL "")
set(FASTGLTF_ENABLE_ASSIMP OFF CACHE INTERNAL "")
set(FASTGLTF_ENABLE_DEPRECATED_EXT OFF CACHE INTERNAL "")
set(FASTGLTF_DISABLE_CUSTOM_MEMORY_POOL OFF CACHE INTERNAL "")
set(FASTGLTF_USE_64BIT_FLOAT OFF CACHE INTERNAL "")
set(FASTGLTF_COMPILE_AS_CPP20 ON CACHE INTERNAL "")
set(FASTGLTF_ENABLE_CPP_MODULES OFF CACHE INTERNAL "")
set(FASTGLTF_USE_STD_MODULE OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(fastgltf)

set_target_properties(
        fastgltf
        PROPERTIES
        FOLDER "third_party")

#-------------------------------------------------------------------------------------------
# fmt
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        fmt
        SYSTEM
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 11.0.2)

set(FMT_DOC OFF CACHE INTERNAL "")
set(FMT_INSTALL OFF CACHE INTERNAL "")
set(FMT_TEST OFF CACHE INTERNAL "")
set(FMT_FUZZ OFF CACHE INTERNAL "")
set(FMT_CUDA_TEST OFF CACHE INTERNAL "")
set(FMT_OS ON CACHE INTERNAL "")
set(FMT_MODULE OFF CACHE INTERNAL "")
set(FMT_SYSTEM_HEADERS OFF CACHE INTERNAL "")
set(FMT_UNICODE ON CACHE INTERNAL "")

FetchContent_MakeAvailable(fmt)

set_target_properties(
        fmt
        PROPERTIES
        FOLDER "third_party")

#-------------------------------------------------------------------------------------------
# glfw
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        glfw
        SYSTEM
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4)

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
set(GLFW_INSTALL OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(glfw)

set_target_properties(
        update_mappings
        PROPERTIES
        FOLDER "third_party")
set_target_properties(
        glfw
        PROPERTIES
        FOLDER "third_party")

#-------------------------------------------------------------------------------------------
# glm
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        glm
        SYSTEM
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1)

set(GLM_BUILD_LIBRARY OFF CACHE INTERNAL "")
set(GLM_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLM_BUILD_INSTALL OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(glm)

target_compile_definitions(
        glm
        INTERFACE
        GLM_ENABLE_EXPERIMENTAL)

set_target_properties(
        glm
        PROPERTIES
        FOLDER "third_party")

#-------------------------------------------------------------------------------------------
# imgui
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        imgui
        SYSTEM
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG origin/docking)

FetchContent_MakeAvailable(imgui)

add_library(
        imgui
        ${CMAKE_BINARY_DIR}/_deps/imgui-src/imconfig.h
        ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui.cpp
        ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui.h
        ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui_demo.cpp
        ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui_draw.cpp
        ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui_internal.h
        ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui_tables.cpp
        ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui_widgets.cpp
        ${CMAKE_BINARY_DIR}/_deps/imgui-src/imstb_rectpack.h
        ${CMAKE_BINARY_DIR}/_deps/imgui-src/imstb_textedit.h
        ${CMAKE_BINARY_DIR}/_deps/imgui-src/imstb_truetype.h)
target_include_directories(
        imgui
        SYSTEM
        PUBLIC
        ${CMAKE_BINARY_DIR}/_deps/imgui-src/)

set_target_properties(
        imgui
        PROPERTIES
        FOLDER "third_party")

#-------------------------------------------------------------------------------------------
# libassert
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        libassert
        SYSTEM
        GIT_REPOSITORY https://github.com/jeremy-rifkin/libassert.git
        GIT_TAG v2.1.2)

FetchContent_MakeAvailable(libassert)

target_compile_definitions(libassert-lib PUBLIC LIBASSERT_PREFIX_ASSERTIONS)

set_target_properties(
        cpptrace-lib
        PROPERTIES
        FOLDER "third_party")
set_target_properties(
        libassert-lib
        PROPERTIES
        FOLDER "third_party")

#-------------------------------------------------------------------------------------------
# meshoptimizer
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        meshoptimizer
        SYSTEM
        GIT_REPOSITORY https://github.com/zeux/meshoptimizer.git
        GIT_TAG v0.22)

set(MESHOPT_BUILD_DEMO OFF CACHE INTERNAL "")
set(MESHOPT_BUILD_GLTFPACK OFF CACHE INTERNAL "")
set(MESHOPT_BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
set(MESHOPT_WERROR OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(meshoptimizer)

set_target_properties(
        meshoptimizer
        PROPERTIES
        FOLDER "third_party")

#-------------------------------------------------------------------------------------------
# spdlog
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        spdlog
        SYSTEM
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.14.1)

set(SPDLOG_BUILD_ALL OFF CACHE INTERNAL "")
set(SPDLOG_BUILD_SHARED OFF CACHE INTERNAL "")
set(SPDLOG_ENABLE_PCH OFF CACHE INTERNAL "")
set(SPDLOG_BUILD_PIC OFF CACHE INTERNAL "")
set(SPDLOG_BUILD_EXAMPLE OFF CACHE INTERNAL "")
set(SPDLOG_BUILD_EXAMPLE_HO OFF CACHE INTERNAL "")
set(SPDLOG_BUILD_TESTS OFF CACHE INTERNAL "")
set(SPDLOG_BUILD_TESTS_HO OFF CACHE INTERNAL "")
set(SPDLOG_BUILD_BENCH OFF CACHE INTERNAL "")
set(SPDLOG_SANITIZE_ADDRESS OFF CACHE INTERNAL "")
set(SPDLOG_BUILD_WARNINGS OFF CACHE INTERNAL "")
set(SPDLOG_SYSTEM_INCLUDES OFF CACHE INTERNAL "")
set(SPDLOG_INSTALL OFF CACHE INTERNAL "")
set(SPDLOG_USE_STD_FORMAT OFF CACHE INTERNAL "")
set(SPDLOG_FMT_EXTERNAL ON CACHE INTERNAL "")
set(SPDLOG_FMT_EXTERNAL_HO OFF CACHE INTERNAL "")
set(SPDLOG_NO_EXCEPTIONS OFF CACHE INTERNAL "")
set(SPDLOG_PREVENT_CHILD_FD OFF CACHE INTERNAL "")
set(SPDLOG_NO_THREAD_ID OFF CACHE INTERNAL "")
set(SPDLOG_NO_TLS OFF CACHE INTERNAL "")
set(SPDLOG_NO_ATOMIC_LEVELS OFF CACHE INTERNAL "")
set(SPDLOG_DISABLE_DEFAULT_LOGGER OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(spdlog)

set_target_properties(
        spdlog
        PROPERTIES
        FOLDER "third_party")

#-------------------------------------------------------------------------------------------
# stb
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        stb
        SYSTEM
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master)

FetchContent_MakeAvailable(stb)

add_subdirectory(stb)

#-------------------------------------------------------------------------------------------
# volk
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        volk
        SYSTEM
        GIT_REPOSITORY https://github.com/zeux/volk.git
        GIT_TAG 1.3.295)

set(VOLK_PULL_IN_VULKAN ON CACHE INTERNAL "")
set(VOLK_INSTALL OFF CACHE INTERNAL "")
set(VOLK_HEADERS_ONLY OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(volk)

set_target_properties(
        volk
        PROPERTIES
        FOLDER "third_party")

#-------------------------------------------------------------------------------------------
# VulkanMemoryAllocator
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        VulkanMemoryAllocator
        SYSTEM
        GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
        GIT_TAG v3.1.0)

set(VMA_ENABLE_INSTALL OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(VulkanMemoryAllocator)

set_target_properties(
        VulkanMemoryAllocator
        PROPERTIES
        FOLDER "third_party")
