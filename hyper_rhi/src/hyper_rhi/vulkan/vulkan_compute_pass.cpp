/*
 * Copyright (c) 2024, SkillerRaptor
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
    VulkanComputePass::VulkanComputePass(
        VulkanGraphicsDevice &graphics_device,
        const VkCommandBuffer command_buffer,
        const ComputePassDescriptor &descriptor)
        : IComputePass(descriptor)
        , m_graphics_device(graphics_device)
        , m_command_buffer(command_buffer)
        , m_pipeline(nullptr)
    {
        m_graphics_device.begin_marker(m_command_buffer, MarkerType::ComputePass, m_label, m_label_color);
    }

    VulkanComputePass::~VulkanComputePass()
    {
        m_graphics_device.end_marker(m_command_buffer);
    }

    void VulkanComputePass::set_pipeline(const std::shared_ptr<IComputePipeline> &pipeline)
    {
        m_pipeline = pipeline;

        const auto vulkan_pipeline = std::dynamic_pointer_cast<VulkanComputePipeline>(m_pipeline);
        const auto layout = std::dynamic_pointer_cast<VulkanPipelineLayout>(m_pipeline->layout());

        const auto &descriptor_sets = m_graphics_device.descriptor_manager().descriptor_sets();

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
        const auto layout = std::dynamic_pointer_cast<VulkanPipelineLayout>(m_pipeline->layout());

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