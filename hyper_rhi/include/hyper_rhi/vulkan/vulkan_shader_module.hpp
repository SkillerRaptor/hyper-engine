/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/prerequisites.hpp>

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

        [[nodiscard]] HE_FORCE_INLINE VkShaderModule shader_module() const
        {
            return m_shader_module;
        }

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkShaderModule m_shader_module;
    };
} // namespace hyper_rhi