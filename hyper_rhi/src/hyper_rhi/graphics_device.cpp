/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/graphics_device.hpp"

#include <hyper_core/assertion.hpp>

#if HE_WINDOWS
#    include "hyper_rhi/d3d12/d3d12_graphics_device.hpp"
#endif

#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"

namespace he::rhi
{
    GraphicsDevice::GraphicsDevice(const GraphicsDeviceDescriptor &descriptor)
        : m_graphics_api(descriptor.graphics_api)
        , m_debug_validation(descriptor.debug_validation)
        , m_debug_label(descriptor.debug_label)
        , m_debug_marker(descriptor.debug_marker)
    {
    }

    std::shared_ptr<GraphicsDevice> GraphicsDevice::create(const GraphicsDeviceDescriptor &descriptor)
    {
        switch (descriptor.graphics_api)
        {
        case GraphicsApi::D3D12:
#if HE_WINDOWS
            return std::make_shared<D3D12GraphicsDevice>(descriptor);
#else
            return nullptr;
#endif
        case GraphicsApi::Vulkan:
            return std::make_shared<VulkanGraphicsDevice>(descriptor);
        default:
            HE_UNREACHABLE();
        }
    }

    GraphicsApi GraphicsDevice::graphics_api() const
    {
        return m_graphics_api;
    }

    bool GraphicsDevice::debug_validation() const
    {
        return m_debug_validation;
    }

    bool GraphicsDevice::debug_label() const
    {
        return m_debug_label;
    }

    bool GraphicsDevice::debug_marker() const
    {
        return m_debug_marker;
    }
} // namespace he::rhi
