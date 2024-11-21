/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/pipeline_layout.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_engine
{
    class VulkanGraphicsDevice;

    class VulkanPipelineLayout final : public IPipelineLayout
    {
    public:
        VulkanPipelineLayout(VulkanGraphicsDevice &graphics_device, const PipelineLayoutDescriptor &descriptor);
        ~VulkanPipelineLayout() override;

        VkPipelineLayout pipeline_layout() const;

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkPipelineLayout m_pipeline_layout;
    };
} // namespace hyper_engine