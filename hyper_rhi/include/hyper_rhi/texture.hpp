/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>

#include <hyper_core/assertion.hpp>
#include <hyper_core/bitmask.hpp>
#include <hyper_core/prerequisites.hpp>

#include "hyper_rhi/resource.hpp"

namespace hyper_rhi
{
    class TextureView;

    enum class Format : uint8_t
    {
        Unknown,

        R8Unorm,
        R8Snorm,
        R8Uint,
        R8Sint,
        R8Srgb,

        Rg8Unorm,
        Rg8Snorm,
        Rg8Uint,
        Rg8Sint,
        Rg8Srgb,

        Rgb8Unorm,
        Rgb8Snorm,
        Rgb8Uint,
        Rgb8Sint,
        Rgb8Srgb,

        Bgr8Unorm,
        Bgr8Snorm,
        Bgr8Uint,
        Bgr8Sint,
        Bgr8Srgb,

        Rgba8Unorm,
        Rgba8Snorm,
        Rgba8Uint,
        Rgba8Sint,
        Rgba8Srgb,

        Bgra8Unorm,
        Bgra8Snorm,
        Bgra8Uint,
        Bgra8Sint,
        Bgra8Srgb,

        R16Unorm,
        R16Snorm,
        R16Uint,
        R16Sint,
        R16Sfloat,

        Rg16Unorm,
        Rg16Snorm,
        Rg16Uint,
        Rg16Sint,
        Rg16Sfloat,

        Rgb16Unorm,
        Rgb16Snorm,
        Rgb16Uint,
        Rgb16Sint,
        Rgb16Sfloat,

        Rgba16Unorm,
        Rgba16Snorm,
        Rgba16Uint,
        Rgba16Sint,
        Rgba16Sfloat,

        R32Uint,
        R32Sint,
        R32Sfloat,

        Rg32Uint,
        Rg32Sint,
        Rg32Sfloat,

        Rgb32Uint,
        Rgb32Sint,
        Rgb32Sfloat,

        Rgba32Uint,
        Rgba32Sint,
        Rgba32Sfloat,

        R64Uint,
        R64Sint,
        R64Sfloat,

        Rg64Uint,
        Rg64Sint,
        Rg64Sfloat,

        Rgb64Uint,
        Rgb64Sint,
        Rgb64Sfloat,

        Rgba64Uint,
        Rgba64Sint,
        Rgba64Sfloat,

        D16Unorm,
        D32Sfloat,
        S8Uint,
        D16UnormS8Uint,
        D24UnormS8Uint,
        D32SfloatS8Uint
    };

    enum class TextureDimension
    {
        Unknown,
        Texture1D,
        Texture1DArray,
        Texture2D,
        Texture2DArray,
        Texture3D,
    };

    enum class TextureUsage : uint8_t
    {
        None = 0,
        Storage = 1 << 0,
        RenderAttachment = 1 << 1,
        ShaderResource = 1 << 2,
    };

    HE_ENABLE_BITMASK_OPERATORS(TextureUsage);

    struct TextureDescriptor
    {
        std::string label;

        uint32_t width = 1;
        uint32_t height = 1;
        uint32_t depth = 1;
        uint32_t array_size = 1;
        uint32_t mip_levels = 1;
        Format format = Format::Unknown;
        TextureDimension dimension = TextureDimension::Unknown;
        TextureUsage usage = TextureUsage::None;
    };

    struct TextureViewDescriptor;

    class Texture : public Resource
    {
    public:
        virtual ~Texture() = default;

        [[nodiscard]] HE_FORCE_INLINE uint32_t width() const
        {
            return m_width;
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t height() const
        {
            return m_height;
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t depth() const
        {
            return m_depth;
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t array_size() const
        {
            return m_array_size;
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t mip_levels() const
        {
            return m_mip_levels;
        }

        [[nodiscard]] HE_FORCE_INLINE Format format() const
        {
            return m_format;
        }

        [[nodiscard]] HE_FORCE_INLINE TextureDimension dimension() const
        {
            return m_dimension;
        }

        [[nodiscard]] HE_FORCE_INLINE TextureUsage usage() const
        {
            return m_usage;
        }

    protected:
        explicit Texture(const TextureDescriptor &descriptor)
            : Resource(descriptor.label)
            , m_width(descriptor.width)
            , m_height(descriptor.height)
            , m_depth(descriptor.depth)
            , m_array_size(descriptor.array_size)
            , m_mip_levels(descriptor.mip_levels)
            , m_format(descriptor.format)
            , m_dimension(descriptor.dimension)
            , m_usage(descriptor.usage)
        {
            HE_ASSERT(m_width > 0);
            HE_ASSERT(m_height > 0);
            HE_ASSERT(m_depth > 0);
            HE_ASSERT(m_array_size > 0);
            HE_ASSERT(m_mip_levels > 0);
            HE_ASSERT(m_format != Format::Unknown);
            HE_ASSERT(m_dimension != TextureDimension::Unknown);
            HE_ASSERT(m_usage != TextureUsage::None);
        }

    protected:
        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_depth;
        uint32_t m_array_size;
        uint32_t m_mip_levels;
        Format m_format;
        TextureDimension m_dimension;
        TextureUsage m_usage;
    };
} // namespace hyper_rhi
