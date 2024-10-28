/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/shader_module.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanShaderModule final : public ShaderModule
    {
    public:
        VulkanShaderModule(VulkanGraphicsDevice &graphics_device, const ShaderModuleDescriptor &descriptor);
        ~VulkanShaderModule() override;

        VkShaderModule shader_module() const;
        std::string_view entry_point() const;

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkShaderModule m_shader_module;
        ShaderType m_shader_type;
        std::string m_entry_point;
    };
} // namespace hyper_rhi