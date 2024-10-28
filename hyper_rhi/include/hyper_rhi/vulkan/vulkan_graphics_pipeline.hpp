/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/graphics_pipeline.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanGraphicsPipeline final : public GraphicsPipeline
    {
    public:
        VulkanGraphicsPipeline(VulkanGraphicsDevice &graphics_device, const GraphicsPipelineDescriptor &descriptor);
        ~VulkanGraphicsPipeline() override;

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkPipeline m_pipeline;
    };
} // namespace hyper_rhi