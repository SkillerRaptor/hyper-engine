/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/sampler.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_engine
{
    class VulkanSampler final : public Sampler
    {
    public:
        VulkanSampler(const SamplerDescriptor &descriptor, ResourceHandle handle, VkSampler sampler);
        ~VulkanSampler() override;

        VkSampler sampler() const;

        static VkFilter get_filter(Filter filter);
        static VkSamplerMipmapMode get_sampler_mipmap_mode(Filter filter);
        static VkSamplerAddressMode get_sampler_address_mode(AddressMode filter);
        static VkBorderColor get_border_color(BorderColor border_color);

    private:
        VkSampler m_sampler = VK_NULL_HANDLE;
    };
} // namespace hyper_engine