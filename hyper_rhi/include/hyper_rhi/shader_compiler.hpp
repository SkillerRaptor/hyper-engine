/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <array>
#include <string>
#include <vector>

#include <wrl.h>
#include <dxcapi.h>

#include "hyper_rhi/shader_module.hpp"

namespace hyper_rhi
{
    struct ShaderDescriptor
    {
        ShaderType type = ShaderType::None;
        std::string entry_name = "main";
        std::vector<uint8_t> data = {};
    };

    struct ShaderData
    {
        std::vector<uint8_t> dxil = {};
        std::vector<uint8_t> spirv = {};
    };

    class ShaderCompiler
    {
    private:
        static constexpr std::array<const wchar_t *, 4> s_compiler_args = {
            L"-HV",
            L"2021",
            L"-Zpr",
            L"-O3",
        };
        static constexpr std::array<const wchar_t *, 5> s_spirv_args = {
            L"-D", L"HE_VULKAN=1", L"-spirv", L"-fvk-use-dx-position-w", L"-fspv-use-legacy-buffer-matrix-order",
        };

    public:
        ShaderCompiler();

        [[nodiscard]] ShaderData compile(const ShaderDescriptor &descriptor) const;

    private:
        Microsoft::WRL::ComPtr<IDxcCompiler3> m_compiler;
        Microsoft::WRL::ComPtr<IDxcUtils> m_utils;
    };
} // namespace hyper_rhi