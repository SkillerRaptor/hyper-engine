/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_shader_module.hpp"

#include <hyper_core/assertion.hpp>
#include <hyper_core/logger.hpp>

#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"

namespace hyper_engine
{
    VulkanShaderModule::VulkanShaderModule(VulkanGraphicsDevice &graphics_device, const ShaderModuleDescriptor &descriptor)
        : IShaderModule(descriptor)
        , m_graphics_device(graphics_device)
        , m_shader_module(VK_NULL_HANDLE)
    {
        const VkShaderModuleCreateInfo shader_module_create_info = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .codeSize = m_bytes.size(),
            .pCode = reinterpret_cast<const uint32_t *>(m_bytes.data()),
        };

        HE_VK_CHECK(vkCreateShaderModule(m_graphics_device.device(), &shader_module_create_info, nullptr, &m_shader_module));
        HE_ASSERT(m_shader_module != VK_NULL_HANDLE);

        const ObjectType object_type = [this]()
        {
            switch (m_type)
            {
            case ShaderType::Compute:
                return ObjectType::ComputeShaderModule;
            case ShaderType::Fragment:
                return ObjectType::FragmentShaderModule;
            case ShaderType::Vertex:
                return ObjectType::VertexShaderModule;
            case ShaderType::None:
            default:
                HE_UNREACHABLE();
            }
        }();

        m_graphics_device.set_object_name(m_shader_module, object_type, m_label);

        // TODO: Add more trace information
        HE_TRACE("Created Shader Module");
    }

    VulkanShaderModule::~VulkanShaderModule()
    {
        m_graphics_device.resource_queue().shader_modules.emplace_back(m_shader_module);
    }

    VkShaderModule VulkanShaderModule::shader_module() const
    {
        return m_shader_module;
    }
} // namespace hyper_engine