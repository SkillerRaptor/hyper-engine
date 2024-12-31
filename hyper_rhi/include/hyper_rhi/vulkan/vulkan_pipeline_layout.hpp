/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/pipeline_layout.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_engine
{
    class VulkanGraphicsDevice;

    class VulkanPipelineLayout final : public PipelineLayout
    {
    public:
        VulkanPipelineLayout(VulkanGraphicsDevice &graphics_device, const PipelineLayoutDescriptor &descriptor);
        ~VulkanPipelineLayout() override;

        std::string_view label() const override;

        uint32_t push_constant_size() const override;

        VkPipelineLayout pipeline_layout() const;

    private:
        VulkanGraphicsDevice &m_graphics_device;

        std::string m_label;

        uint32_t m_push_constant_size;

        VkPipelineLayout m_pipeline_layout;
    };
} // namespace hyper_engine