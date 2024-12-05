/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/compute_pass.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_engine
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

        std::string_view label() const override;
        LabelColor label_color() const override;

        VkCommandBuffer command_buffer() const;

    private:
        VulkanGraphicsDevice &m_graphics_device;

        std::string m_label;
        LabelColor m_label_color;

        VkCommandBuffer m_command_buffer;

        std::shared_ptr<IComputePipeline> m_pipeline;
    };
} // namespace hyper_engine