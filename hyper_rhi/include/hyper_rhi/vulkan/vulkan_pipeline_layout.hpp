/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/pipeline_layout.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanPipelineLayout final : public PipelineLayout
    {
    public:
        VulkanPipelineLayout(VulkanGraphicsDevice &graphics_device, const PipelineLayoutDescriptor &descriptor);
        ~VulkanPipelineLayout() override;

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkPipelineLayout m_pipeline_layout;
        uint32_t m_push_constant_size;
    };
} // namespace hyper_rhi