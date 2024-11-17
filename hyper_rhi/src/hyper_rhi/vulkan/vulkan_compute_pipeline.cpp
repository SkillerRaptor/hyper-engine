/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_compute_pipeline.hpp"

#include <hyper_core/assertion.hpp>
#include <hyper_core/logger.hpp>

#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"
#include "hyper_rhi/vulkan/vulkan_pipeline_layout.hpp"
#include "hyper_rhi/vulkan/vulkan_shader_module.hpp"

namespace he::rhi
{
    VulkanComputePipeline::VulkanComputePipeline(VulkanGraphicsDevice &graphics_device, const ComputePipelineDescriptor &descriptor)
        : ComputePipeline(descriptor)
        , m_graphics_device(graphics_device)
        , m_pipeline(VK_NULL_HANDLE)
    {
        const auto layout = std::dynamic_pointer_cast<VulkanPipelineLayout>(descriptor.layout);
        const auto shader_module = std::dynamic_pointer_cast<VulkanShaderModule>(descriptor.shader);

        const VkPipelineShaderStageCreateInfo shader_stage_create_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_COMPUTE_BIT,
            .module = shader_module->shader_module(),
            .pName = shader_module->entry_name().data(),
            .pSpecializationInfo = nullptr,
        };

        const VkComputePipelineCreateInfo compute_pipeline_create_info = {
            .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = shader_stage_create_info,
            .layout = layout->pipeline_layout(),
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex = -1,
        };

        HE_VK_CHECK(
            vkCreateComputePipelines(m_graphics_device.device(), VK_NULL_HANDLE, 1, &compute_pipeline_create_info, nullptr, &m_pipeline));
        HE_ASSERT(m_pipeline != VK_NULL_HANDLE);

        m_graphics_device.set_object_name(m_pipeline, ObjectType::ComputePipeline, m_label);

        // TODO: Add more trace information
        HE_TRACE("Created Compute Pipeline");
    }

    VulkanComputePipeline::~VulkanComputePipeline()
    {
        m_graphics_device.resource_queue().compute_pipelines.emplace_back(m_pipeline);
    }

    VkPipeline VulkanComputePipeline::pipeline() const
    {
        return m_pipeline;
    }
} // namespace he::rhi