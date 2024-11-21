/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/shader_module.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace he::rhi
{
    class VulkanGraphicsDevice;

    class VulkanShaderModule final : public IShaderModule
    {
    public:
        VulkanShaderModule(VulkanGraphicsDevice &graphics_device, const ShaderModuleDescriptor &descriptor);
        ~VulkanShaderModule() override;

        VkShaderModule shader_module() const;

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkShaderModule m_shader_module;
    };
} // namespace he::rhi