/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/shader_module.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_engine
{
    class VulkanShaderModule final : public ShaderModule
    {
    public:
        VulkanShaderModule(const ShaderModuleDescriptor &descriptor, VkShaderModule shader_module);
        ~VulkanShaderModule() override;

        VkShaderModule shader_module() const;

    private:
        VkShaderModule m_shader_module = VK_NULL_HANDLE;
    };
} // namespace hyper_engine