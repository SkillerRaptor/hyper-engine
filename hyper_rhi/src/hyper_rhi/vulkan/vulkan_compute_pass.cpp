/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_compute_pass.hpp"

#include <hyper_core/logger.hpp>

#include "hyper_rhi/vulkan/vulkan_compute_pipeline.hpp"
#include "hyper_rhi/vulkan/vulkan_descriptor_manager.hpp"
#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"
#include "hyper_rhi/vulkan/vulkan_pipeline_layout.hpp"

namespace hyper_engine
{
    VulkanComputePass::VulkanComputePass(const ComputePassDescriptor &descriptor, const VkCommandBuffer command_buffer)
        : ComputePass(descriptor)
        , m_command_buffer(command_buffer)
    {
        const VulkanGraphicsDevice *graphics_device = static_cast<VulkanGraphicsDevice *>(g_env.graphics_device);
        graphics_device->begin_marker(m_command_buffer, MarkerType::ComputePass, m_label, m_label_color);
    }

    VulkanComputePass::~VulkanComputePass()
    {
        const VulkanGraphicsDevice *graphics_device = static_cast<VulkanGraphicsDevice *>(g_env.graphics_device);
        graphics_device->end_marker(m_command_buffer);
    }

    void VulkanComputePass::set_pipeline(const RefPtr<ComputePipeline> &pipeline)
    {
        m_pipeline = pipeline;

        const RefPtr<VulkanComputePipeline> vulkan_pipeline = std::static_pointer_cast<VulkanComputePipeline>(m_pipeline);
        const RefPtr<VulkanPipelineLayout> layout = std::static_pointer_cast<VulkanPipelineLayout>(m_pipeline->layout());

        VulkanGraphicsDevice *graphics_device = static_cast<VulkanGraphicsDevice *>(g_env.graphics_device);
        const VulkanDescriptorManager &descriptor_manager = static_cast<VulkanDescriptorManager &>(graphics_device->descriptor_manager());
        const auto &descriptor_sets = descriptor_manager.descriptor_sets();

        vkCmdBindDescriptorSets(
            m_command_buffer,
            VK_PIPELINE_BIND_POINT_COMPUTE,
            layout->pipeline_layout(),
            0,
            static_cast<uint32_t>(descriptor_sets.size()),
            descriptor_sets.data(),
            0,
            nullptr);

        vkCmdBindPipeline(m_command_buffer, VK_PIPELINE_BIND_POINT_COMPUTE, vulkan_pipeline->pipeline());
    }

    void VulkanComputePass::set_push_constants(const void *data, const size_t data_size) const
    {
        const RefPtr<VulkanPipelineLayout> layout = std::static_pointer_cast<VulkanPipelineLayout>(m_pipeline->layout());

        vkCmdPushConstants(m_command_buffer, layout->pipeline_layout(), VK_SHADER_STAGE_ALL, 0, static_cast<uint32_t>(data_size), data);
    }

    void VulkanComputePass::dispatch(const uint32_t x, const uint32_t y, const uint32_t z) const
    {
        vkCmdDispatch(m_command_buffer, x, y, z);
    }

    VkCommandBuffer VulkanComputePass::command_buffer() const
    {
        return m_command_buffer;
    }
} // namespace hyper_engine