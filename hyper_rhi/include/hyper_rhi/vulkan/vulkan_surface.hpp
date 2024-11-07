/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/prerequisites.hpp>

#include "hyper_rhi/surface.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"
#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"

namespace hyper_rhi
{
    class VulkanSurface final : public Surface
    {
    public:
        VulkanSurface(VulkanGraphicsDevice &graphics_device, const hyper_platform::Window &window);
        ~VulkanSurface() override;

        void rebuild();

        [[nodiscard]] HE_FORCE_INLINE VkSwapchainKHR swapchain() const
        {
            return m_swapchain;
        }

        HE_FORCE_INLINE void set_texture_index(const uint32_t texture_index)
        {
            m_texture_index = texture_index;
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t texture_index() const
        {
            return m_texture_index;
        }

        [[nodiscard]] HE_FORCE_INLINE std::shared_ptr<Texture> current_texture() const override
        {
            return m_textures[m_texture_index];
        }

    private:
        void create_surface(const hyper_platform::Window &window);
        void create_swapchain();
        void create_textures();
        void destroy();

        static VkExtent2D choose_extent(uint32_t width, uint32_t height, const VkSurfaceCapabilitiesKHR &capabilities);
        static VkSurfaceFormatKHR choose_format(const std::vector<VkSurfaceFormatKHR> &formats);
        static VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR> &present_modes);

    private:
        VulkanGraphicsDevice &m_graphics_device;

        VkSurfaceKHR m_surface;
        VkSwapchainKHR m_swapchain;
        VkFormat m_format;

        uint32_t m_texture_index;
        std::vector<std::shared_ptr<Texture>> m_textures;
        std::vector<std::shared_ptr<TextureView>> m_texture_views;
    };
} // namespace hyper_rhi