/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/prerequisites.hpp>

#include "hyper_rhi/compute_pipeline.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanComputePipeline final : public ComputePipeline
    {
    public:
        VulkanComputePipeline(VulkanGraphicsDevice &graphics_device, const ComputePipelineDescriptor &descriptor);
        ~VulkanComputePipeline() override;

        [[nodiscard]] HE_FORCE_INLINE VkPipeline pipeline() const
        {
            return m_pipeline;
        }

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkPipeline m_pipeline;
    };

} // namespace hyper_rhi