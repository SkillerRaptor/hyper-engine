/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/compute_pipeline.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_engine
{
    class VulkanGraphicsDevice;

    class VulkanComputePipeline final : public IComputePipeline
    {
    public:
        VulkanComputePipeline(VulkanGraphicsDevice &graphics_device, const ComputePipelineDescriptor &descriptor);
        ~VulkanComputePipeline() override;

        std::string_view label() const override;

        const std::shared_ptr<IPipelineLayout> &layout() const override;
        const std::shared_ptr<IShaderModule> &shader() const override;

        VkPipeline pipeline() const;

    private:
        VulkanGraphicsDevice &m_graphics_device;

        std::string m_label;

        std::shared_ptr<IPipelineLayout> m_layout;
        std::shared_ptr<IShaderModule> m_shader;

        VkPipeline m_pipeline;
    };

} // namespace hyper_engine