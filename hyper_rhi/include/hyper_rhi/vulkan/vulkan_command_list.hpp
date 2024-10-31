/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/command_list.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanCommandList final : public CommandList
    {
    public:
        explicit VulkanCommandList(VulkanGraphicsDevice &graphics_device);

        void begin() override;
        void end() override;

        void insert_barriers(const Barriers &barriers) const override;

        [[nodiscard]] std::shared_ptr<RenderPass> begin_render_pass(const RenderPassDescriptor &descriptor) const override;

        [[nodiscard]] VkCommandBuffer command_buffer() const;

    private:
        [[nodiscard]] static VkPipelineStageFlags2 get_pipeline_stage(BarrierPipelineStage barrier_pipeline_stage);
        [[nodiscard]] static VkAccessFlags2 get_access(BarrierAccess barrier_access);
        [[nodiscard]] static VkImageLayout get_image_layout(BarrierTextureLayout texture_layout);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkCommandBuffer m_command_buffer;
    };
} // namespace hyper_rhi