/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <array>
#include <stack>

#include "hyper_rhi/buffer.hpp"
#include "hyper_rhi/resource_handle.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_rhi
{
    class VulkanGraphicsDevice;

    class VulkanDescriptorManager
    {
    private:
        static constexpr std::array<VkDescriptorType, 3> s_descriptor_types = {
            VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
            VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
        };

    public:
        explicit VulkanDescriptorManager(VulkanGraphicsDevice &graphics_device);
        ~VulkanDescriptorManager();

        void set_dynamic_buffer(VkBuffer buffer, uint32_t slot);

        [[nodiscard]] ResourceHandle allocate_buffer_handle(VkBuffer buffer);
        void retire_handle(ResourceHandle handle);

        [[nodiscard]] const std::array<VkDescriptorSetLayout, s_descriptor_types.size()> &descriptor_set_layouts() const;
        [[nodiscard]] const std::array<VkDescriptorSet, s_descriptor_types.size()> &descriptor_sets() const;

    private:
        void find_descriptor_counts();
        void create_descriptor_pool();
        void create_descriptor_set_layouts();
        void create_descriptor_sets();

        ResourceHandle fetch_handle();

    private:
        VulkanGraphicsDevice &m_graphics_device;

        std::array<uint32_t, s_descriptor_types.size()> m_descriptor_counts;

        VkDescriptorPool m_descriptor_pool;
        std::array<VkDescriptorSetLayout, s_descriptor_types.size()> m_descriptor_set_layouts;
        std::array<VkDescriptorSet, s_descriptor_types.size()> m_descriptor_sets;

        std::stack<ResourceHandle> m_recycled_descriptors;
        uint32_t m_current_index;
    };
} // namespace hyper_rhi