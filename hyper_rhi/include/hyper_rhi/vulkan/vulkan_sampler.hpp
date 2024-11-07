/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/prerequisites.hpp>

#include "hyper_rhi/sampler.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanSampler final : public Sampler
    {
    public:
        VulkanSampler(VulkanGraphicsDevice &graphics_device, const SamplerDescriptor &descriptor);
        ~VulkanSampler() override;

        [[nodiscard]] static VkFilter get_filter(Filter filter);
        [[nodiscard]] static VkSamplerMipmapMode get_sampler_mipmap_mode(Filter filter);
        [[nodiscard]] static VkSamplerAddressMode get_sampler_address_mode(SamplerAddressMode filter);
        [[nodiscard]] static VkBorderColor get_border_color(BorderColor border_color);

        [[nodiscard]] HE_FORCE_INLINE VkSampler sampler() const
        {
            return m_sampler;
        }

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkSampler m_sampler;
    };
} // namespace hyper_rhi