/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/vulkan/vulkan_descriptor_manager.hpp"

#include <hyper_core/assertion.hpp>
#include <hyper_core/logger.hpp>

#include "hyper_rhi/vulkan/vulkan_buffer.hpp"
#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"

namespace hyper_engine
{
    VulkanDescriptorManager::VulkanDescriptorManager(VulkanGraphicsDevice &graphics_device)
        : m_graphics_device(graphics_device)
        , m_descriptor_counts({})
        , m_descriptor_pool(nullptr)
        , m_descriptor_set_layouts()
        , m_descriptor_sets()
        , m_recycled_descriptors()
        , m_current_index(0)
    {
        find_descriptor_counts();
        create_descriptor_pool();
        create_descriptor_set_layouts();
        create_descriptor_sets();
    }

    VulkanDescriptorManager::~VulkanDescriptorManager()
    {
        for (const VkDescriptorSetLayout &descriptor_set_layout : m_descriptor_set_layouts)
        {
            vkDestroyDescriptorSetLayout(m_graphics_device.device(), descriptor_set_layout, nullptr);
        }

        vkDestroyDescriptorPool(m_graphics_device.device(), m_descriptor_pool, nullptr);
    }

    void VulkanDescriptorManager::set_buffer(const VkBuffer buffer, const uint32_t slot) const
    {
        const VkDescriptorBufferInfo buffer_info = {
            .buffer = buffer,
            .offset = 0,
            .range = VK_WHOLE_SIZE,
        };

        const VkWriteDescriptorSet descriptor_write = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = m_descriptor_sets[0],
            .dstBinding = 0,
            .dstArrayElement = slot,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            .pImageInfo = nullptr,
            .pBufferInfo = &buffer_info,
            .pTexelBufferView = nullptr,
        };

        vkUpdateDescriptorSets(m_graphics_device.device(), 1, &descriptor_write, 0, nullptr);
    }

    void VulkanDescriptorManager::set_sampled_image(const VkImageView image_view, const VkImageLayout image_layout, const uint32_t slot) const
    {
        const VkDescriptorImageInfo image_info = {
            .sampler = VK_NULL_HANDLE,
            .imageView = image_view,
            .imageLayout = image_layout,
        };

        const VkWriteDescriptorSet descriptor_write = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = m_descriptor_sets[1],
            .dstBinding = 0,
            .dstArrayElement = slot,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
            .pImageInfo = &image_info,
            .pBufferInfo = nullptr,
            .pTexelBufferView = nullptr,
        };

        vkUpdateDescriptorSets(m_graphics_device.device(), 1, &descriptor_write, 0, nullptr);
    }

    void VulkanDescriptorManager::set_storage_image(const VkImageView image_view, const VkImageLayout image_layout, const uint32_t slot) const
    {
        const VkDescriptorImageInfo image_info = {
            .sampler = VK_NULL_HANDLE,
            .imageView = image_view,
            .imageLayout = image_layout,
        };

        const VkWriteDescriptorSet descriptor_write = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = m_descriptor_sets[2],
            .dstBinding = 0,
            .dstArrayElement = slot,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
            .pImageInfo = &image_info,
            .pBufferInfo = nullptr,
            .pTexelBufferView = nullptr,
        };

        vkUpdateDescriptorSets(m_graphics_device.device(), 1, &descriptor_write, 0, nullptr);
    }

    void VulkanDescriptorManager::set_sampler(const VkSampler sampler, const uint32_t slot) const
    {
        const VkDescriptorImageInfo image_info = {
            .sampler = sampler,
            .imageView = VK_NULL_HANDLE,
            .imageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        };

        const VkWriteDescriptorSet descriptor_write = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = m_descriptor_sets[3],
            .dstBinding = 0,
            .dstArrayElement = slot,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER,
            .pImageInfo = &image_info,
            .pBufferInfo = nullptr,
            .pTexelBufferView = nullptr,
        };

        vkUpdateDescriptorSets(m_graphics_device.device(), 1, &descriptor_write, 0, nullptr);
    }

    ResourceHandle VulkanDescriptorManager::allocate_buffer_handle(const VkBuffer buffer)
    {
        const ResourceHandle handle = fetch_handle();
        set_buffer(buffer, handle.handle());
        return handle;
    }

    ResourceHandle VulkanDescriptorManager::allocate_sampled_image_handle(const VkImageView image_view, const VkImageLayout image_layout)
    {
        const ResourceHandle handle = fetch_handle();
        set_sampled_image(image_view, image_layout, handle.handle());
        return handle;
    }

    ResourceHandle VulkanDescriptorManager::allocate_storage_image_handle(const VkImageView image_view, const VkImageLayout image_layout)
    {
        const ResourceHandle handle = fetch_handle();
        set_storage_image(image_view, image_layout, handle.handle());
        return handle;
    }

    ResourceHandle VulkanDescriptorManager::allocate_sampler_handle(const VkSampler sampler)
    {
        const ResourceHandle handle = fetch_handle();
        set_sampler(sampler, handle.handle());
        return handle;
    }

    void VulkanDescriptorManager::retire_handle(const ResourceHandle handle)
    {
        if (handle.handle() == std::numeric_limits<uint32_t>::max())
        {
            return;
        }

        m_recycled_descriptors.push(handle);
    }

    const std::array<uint32_t, VulkanDescriptorManager::s_descriptor_types.size()> &VulkanDescriptorManager::descriptor_counts() const
    {
        return m_descriptor_counts;
    }

    VkDescriptorPool VulkanDescriptorManager::descriptor_pool() const
    {
        return m_descriptor_pool;
    }

    const std::array<VkDescriptorSetLayout, VulkanDescriptorManager::s_descriptor_types.size()> &
        VulkanDescriptorManager::descriptor_set_layouts() const
    {
        return m_descriptor_set_layouts;
    }

    const std::array<VkDescriptorSet, VulkanDescriptorManager::s_descriptor_types.size()> &VulkanDescriptorManager::descriptor_sets() const
    {
        return m_descriptor_sets;
    }

    void VulkanDescriptorManager::find_descriptor_counts()
    {
        VkPhysicalDeviceProperties properties = {};
        vkGetPhysicalDeviceProperties(m_graphics_device.physical_device(), &properties);

        for (size_t index = 0; index != s_descriptor_types.size(); ++index)
        {
            const VkDescriptorType &descriptor_type = s_descriptor_types[index];

            const uint32_t limit = [&properties, &descriptor_type]()
            {
                switch (descriptor_type)
                {
                case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
                    return properties.limits.maxDescriptorSetStorageBuffers;
                case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
                    return properties.limits.maxDescriptorSetSampledImages;
                case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
                    return properties.limits.maxDescriptorSetStorageImages;
                case VK_DESCRIPTOR_TYPE_SAMPLER:
                    return properties.limits.maxDescriptorSetSamplers;
                default:
                    HE_UNREACHABLE();
                }
            }();

            const uint32_t descriptor_count = limit > GraphicsDevice::s_descriptor_limit ? GraphicsDevice::s_descriptor_limit : limit;

            m_descriptor_counts[index] = descriptor_count;

            HE_TRACE("Found descriptor count for {} of {}", HE_VK_TYPE_TO_STRING(VkDescriptorType, descriptor_type), descriptor_count);
        }
    }

    void VulkanDescriptorManager::create_descriptor_pool()
    {
        std::array<VkDescriptorPoolSize, s_descriptor_types.size()> descriptor_pool_sizes = {};
        for (size_t index = 0; index < s_descriptor_types.size(); index++)
        {
            const VkDescriptorType &descriptor_type = s_descriptor_types[index];
            const uint32_t descriptor_count = m_descriptor_counts[index];

            const VkDescriptorPoolSize descriptor_pool_size = {
                .type = descriptor_type,
                .descriptorCount = descriptor_count,
            };

            descriptor_pool_sizes[index] = descriptor_pool_size;
        }

        const VkDescriptorPoolCreateInfo descriptor_pool_create_info = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT,
            .maxSets = static_cast<uint32_t>(s_descriptor_types.size()),
            .poolSizeCount = static_cast<uint32_t>(descriptor_pool_sizes.size()),
            .pPoolSizes = descriptor_pool_sizes.data(),
        };

        HE_VK_CHECK(vkCreateDescriptorPool(m_graphics_device.device(), &descriptor_pool_create_info, nullptr, &m_descriptor_pool));
        HE_ASSERT(m_descriptor_pool != VK_NULL_HANDLE);

        HE_TRACE("Created Descriptor Pool with {} sets", s_descriptor_types.size());
    }

    void VulkanDescriptorManager::create_descriptor_set_layouts()
    {
        for (size_t index = 0; index != s_descriptor_types.size(); ++index)
        {
            const VkDescriptorType &descriptor_type = s_descriptor_types[index];
            const uint32_t descriptor_count = m_descriptor_counts[index];

            const VkDescriptorSetLayoutBinding descriptor_set_layout_binding = {
                .binding = 0,
                .descriptorType = descriptor_type,
                .descriptorCount = descriptor_count,
                .stageFlags = VK_SHADER_STAGE_ALL,
                .pImmutableSamplers = nullptr,
            };

            constexpr VkDescriptorBindingFlags descriptor_binding_flags = VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT |
                                                                          VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT |
                                                                          VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT;

            VkDescriptorSetLayoutBindingFlagsCreateInfo descriptor_set_layout_binding_flags_info = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
                .pNext = nullptr,
                .bindingCount = 1,
                .pBindingFlags = &descriptor_binding_flags,
            };

            const VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                .pNext = &descriptor_set_layout_binding_flags_info,
                .flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT,
                .bindingCount = 1,
                .pBindings = &descriptor_set_layout_binding,
            };

            HE_VK_CHECK(vkCreateDescriptorSetLayout(
                m_graphics_device.device(), &descriptor_set_layout_create_info, nullptr, &m_descriptor_set_layouts[index]));
            HE_ASSERT(m_descriptor_set_layouts[index] != VK_NULL_HANDLE);

            HE_TRACE("Created Descriptor Set Layout #{} for {}", index, HE_VK_TYPE_TO_STRING(VkDescriptorType, descriptor_type));
        }
    }

    void VulkanDescriptorManager::create_descriptor_sets()
    {
        for (size_t index = 0; index != m_descriptor_set_layouts.size(); ++index)
        {
            const VkDescriptorSetLayout &descriptor_set_layout = m_descriptor_set_layouts[index];
            const VkDescriptorType &descriptor_type = s_descriptor_types[index];
            const uint32_t descriptor_count = m_descriptor_counts[index];

            VkDescriptorSetVariableDescriptorCountAllocateInfo descriptor_set_variable_descriptor_count_info = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,
                .pNext = nullptr,
                .descriptorSetCount = 1,
                .pDescriptorCounts = &descriptor_count,
            };

            const VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                .pNext = &descriptor_set_variable_descriptor_count_info,
                .descriptorPool = m_descriptor_pool,
                .descriptorSetCount = 1,
                .pSetLayouts = &descriptor_set_layout,
            };

            HE_VK_CHECK(vkAllocateDescriptorSets(m_graphics_device.device(), &descriptor_set_allocate_info, &m_descriptor_sets[index]));
            HE_ASSERT(m_descriptor_sets[index] != VK_NULL_HANDLE);

            HE_TRACE("Allocated Descriptor Set #{} for {}", index, HE_VK_TYPE_TO_STRING(VkDescriptorType, descriptor_type));
        }
    }

    ResourceHandle VulkanDescriptorManager::fetch_handle()
    {
        if (m_recycled_descriptors.empty())
        {
            return ResourceHandle(m_current_index++);
        }

        const ResourceHandle handle = m_recycled_descriptors.top();
        m_recycled_descriptors.pop();
        return handle;
    }
} // namespace hyper_engine