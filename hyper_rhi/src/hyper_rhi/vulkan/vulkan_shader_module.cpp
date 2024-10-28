/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_shader_module.hpp"

#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"

namespace hyper_rhi
{
    VulkanShaderModule::VulkanShaderModule(VulkanGraphicsDevice &graphics_device, const ShaderModuleDescriptor &descriptor)
        : m_graphics_device(graphics_device)
        , m_shader_module(VK_NULL_HANDLE)
        , m_shader_type(descriptor.type)
        , m_entry_point(descriptor.entry_name)
    {
        const VkShaderModuleCreateInfo shader_module_create_info = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .codeSize = descriptor.bytes.size() * sizeof(uint8_t),
            .pCode = reinterpret_cast<const uint32_t *>(descriptor.bytes.data()),
        };

        HE_VK_CHECK(vkCreateShaderModule(m_graphics_device.device(), &shader_module_create_info, nullptr, &m_shader_module));
        HE_ASSERT(m_shader_module != VK_NULL_HANDLE);

        // TODO: Log debug name

        HE_TRACE("Created Shader Module");
    }

    VulkanShaderModule::~VulkanShaderModule()
    {
        // TODO: Queue it
        vkDestroyShaderModule(m_graphics_device.device(), m_shader_module, nullptr);
    }

    VkShaderModule VulkanShaderModule::shader_module() const
    {
        return m_shader_module;
    }

    std::string_view VulkanShaderModule::entry_point() const
    {
        return m_entry_point;
    }
} // namespace hyper_rhi