/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <vector>

#include "hyper_rhi/surface.hpp"
#include "hyper_rhi/vulkan/vulkan_common.hpp"

namespace hyper_engine
{
    class VulkanGraphicsDevice;
    class Window;

    class VulkanSurface final : public ISurface
    {
    public:
        VulkanSurface(VulkanGraphicsDevice &graphics_device);
        ~VulkanSurface() override;

        void rebuild();

        VkSwapchainKHR swapchain() const;
        void set_texture_index(uint32_t texture_index);
        uint32_t texture_index() const;
        uint32_t min_image_count() const override;
        uint32_t image_count() const override;
        Format format() const override;
        std::shared_ptr<ITexture> current_texture() const override;
        std::shared_ptr<ITextureView> current_texture_view() const override;

    private:
        void create_surface();
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
        uint32_t m_min_image_count;
        uint32_t m_image_count;
        VkFormat m_format;

        uint32_t m_texture_index;
        std::vector<std::shared_ptr<ITexture>> m_textures;
        std::vector<std::shared_ptr<ITextureView>> m_texture_views;
    };
} // namespace hyper_engine