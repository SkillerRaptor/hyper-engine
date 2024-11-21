/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/sampler.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace he::rhi
{
    class VulkanGraphicsDevice;

    class VulkanSampler final : public ISampler
    {
    public:
        VulkanSampler(VulkanGraphicsDevice &graphics_device, const SamplerDescriptor &descriptor);
        ~VulkanSampler() override;

        VkSampler sampler() const;

        static VkFilter get_filter(Filter filter);
        static VkSamplerMipmapMode get_sampler_mipmap_mode(Filter filter);
        static VkSamplerAddressMode get_sampler_address_mode(AddressMode filter);
        static VkBorderColor get_border_color(BorderColor border_color);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkSampler m_sampler;
    };
} // namespace he::rhi