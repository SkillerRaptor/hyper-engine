/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_pipeline_layout.hpp"

#include <hyper_core/assertion.hpp>
#include <hyper_core/logger.hpp>

#include "hyper_rhi/vulkan/vulkan_descriptor_manager.hpp"
#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"

namespace hyper_engine
{
    VulkanPipelineLayout::VulkanPipelineLayout(VulkanGraphicsDevice &graphics_device, const PipelineLayoutDescriptor &descriptor)
        : m_graphics_device(graphics_device)
        , m_label(descriptor.label)
        , m_push_constant_size(descriptor.push_constant_size)
        , m_pipeline_layout(VK_NULL_HANDLE)
    {
        const auto &descriptor_set_layouts = m_graphics_device.descriptor_manager().descriptor_set_layouts();

        const VkPushConstantRange push_constant_range = {
            .stageFlags = VK_SHADER_STAGE_ALL,
            .offset = 0,
            .size = m_push_constant_size,
        };

        const VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .setLayoutCount = static_cast<uint32_t>(descriptor_set_layouts.size()),
            .pSetLayouts = descriptor_set_layouts.data(),
            .pushConstantRangeCount = static_cast<uint32_t>(m_push_constant_size == 0 ? 0 : 1),
            .pPushConstantRanges = m_push_constant_size == 0 ? nullptr : &push_constant_range,
        };

        HE_VK_CHECK(vkCreatePipelineLayout(m_graphics_device.device(), &pipeline_layout_create_info, nullptr, &m_pipeline_layout));
        HE_ASSERT(m_pipeline_layout != VK_NULL_HANDLE);

        m_graphics_device.set_object_name(m_pipeline_layout, ObjectType::PipelineLayout, m_label);

        // TODO: Add more trace information
        HE_TRACE("Created Pipeline Layout");
    }

    VulkanPipelineLayout::~VulkanPipelineLayout()
    {
        m_graphics_device.resource_queue().pipeline_layouts.emplace_back(m_pipeline_layout);
    }

    std::string_view VulkanPipelineLayout::label() const
    {
        return m_label;
    }

    uint32_t VulkanPipelineLayout::push_constant_size() const
    {
        return m_push_constant_size;
    }

    VkPipelineLayout VulkanPipelineLayout::pipeline_layout() const
    {
        return m_pipeline_layout;
    }
} // namespace hyper_engine