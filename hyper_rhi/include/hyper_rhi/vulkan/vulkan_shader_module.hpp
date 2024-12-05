/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/shader_module.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_engine
{
    class VulkanGraphicsDevice;

    class VulkanShaderModule final : public IShaderModule
    {
    public:
        VulkanShaderModule(VulkanGraphicsDevice &graphics_device, const ShaderModuleDescriptor &descriptor);
        ~VulkanShaderModule() override;

        std::string_view label() const override;

        ShaderType type() const override;
        std::string_view entry_name() const override;
        const std::vector<uint8_t> &bytes() const override;

        VkShaderModule shader_module() const;

    private:
        VulkanGraphicsDevice &m_graphics_device;

        std::string m_label;

        ShaderType m_type;
        std::string m_entry_name;
        std::vector<uint8_t> m_bytes;

        VkShaderModule m_shader_module;
    };
} // namespace hyper_engine