/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/compute_pipeline.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace he::rhi
{
    class VulkanGraphicsDevice;

    class VulkanComputePipeline final : public IComputePipeline
    {
    public:
        VulkanComputePipeline(VulkanGraphicsDevice &graphics_device, const ComputePipelineDescriptor &descriptor);
        ~VulkanComputePipeline() override;

        VkPipeline pipeline() const;

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkPipeline m_pipeline;
    };

} // namespace he::rhi