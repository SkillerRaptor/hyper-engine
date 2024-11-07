/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/prerequisites.hpp>

#include "hyper_rhi/compute_pass.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanComputePass final : public ComputePass
    {
    public:
        VulkanComputePass(VulkanGraphicsDevice &graphics_device, VkCommandBuffer command_buffer, const ComputePassDescriptor &descriptor);
        ~VulkanComputePass() override;

        void set_pipeline(const std::shared_ptr<ComputePipeline> &pipeline) override;
        void set_push_constants(const void *data, size_t data_size) const override;

        void dispatch(uint32_t x, uint32_t y, uint32_t z) const override;

        [[nodiscard]] HE_FORCE_INLINE VkCommandBuffer command_buffer() const
        {
            return m_command_buffer;
        }

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkCommandBuffer m_command_buffer;

        std::shared_ptr<ComputePipeline> m_pipeline;
    };
} // namespace hyper_rhi