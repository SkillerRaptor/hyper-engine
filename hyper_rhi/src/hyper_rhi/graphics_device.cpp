/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_rhi/graphics_device.hpp"

#include <hyper_core/assertion.hpp>

#if HE_WINDOWS
// #    include "hyper_rhi/d3d12/d3d12_graphics_device.hpp"
#endif

#include "hyper_rhi/vulkan/vulkan_graphics_device.hpp"

namespace hyper_engine
{
    GraphicsDevice *GraphicsDevice::create(const GraphicsDeviceDescriptor &descriptor)
    {
        switch (descriptor.graphics_api)
        {
        case GraphicsApi::D3D12:
#if HE_WINDOWS
            // return new D3D12GraphicsDevice(descriptor);
            HE_PANIC();
#else
            return nullptr;
#endif
        case GraphicsApi::Vulkan:
            return new VulkanGraphicsDevice(descriptor);
        default:
            HE_UNREACHABLE();
        }
    }
} // namespace hyper_engine
