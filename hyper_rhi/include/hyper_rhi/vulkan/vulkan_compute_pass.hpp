/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/compute_pass.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace he::rhi
{
    class VulkanGraphicsDevice;

    class VulkanComputePass final : public IComputePass
    {
    public:
        VulkanComputePass(VulkanGraphicsDevice &graphics_device, VkCommandBuffer command_buffer, const ComputePassDescriptor &descriptor);
        ~VulkanComputePass() override;

        void set_pipeline(const std::shared_ptr<IComputePipeline> &pipeline) override;
        void set_push_constants(const void *data, size_t data_size) const override;

        void dispatch(uint32_t x, uint32_t y, uint32_t z) const override;

        VkCommandBuffer command_buffer() const;

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkCommandBuffer m_command_buffer;

        std::shared_ptr<IComputePipeline> m_pipeline;
    };
} // namespace he::rhi