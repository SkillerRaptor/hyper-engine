/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>
#include <string>

#include <hyper_core/bitmask.hpp>

#include "hyper_rhi/resource.hpp"
#include "hyper_rhi/resource_handle.hpp"

namespace hyper_rhi
{
    class TextureView;

    enum class TextureFormat
    {
        Unknown,
        B8G8R8A8_Srgb,
        D32_SFloat,
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
        ShaderResource = 1 << 0,
        RenderTarget = 1 << 1,
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
        TextureFormat format = TextureFormat::Unknown;
        TextureDimension dimension = TextureDimension::Unknown;
        TextureUsage usage = TextureUsage::None;
    };

    class Texture : public Resource
    {
    public:
        virtual ~Texture() = default;

        [[nodiscard]] uint32_t width() const;
        [[nodiscard]] uint32_t height() const;
        [[nodiscard]] uint32_t depth() const;
        [[nodiscard]] uint32_t array_size() const;
        [[nodiscard]] uint32_t mip_levels() const;
        [[nodiscard]] TextureFormat format() const;
        [[nodiscard]] TextureDimension dimension() const;
        [[nodiscard]] TextureUsage usage() const;
        [[nodiscard]] TextureView &view() const;

    protected:
        explicit Texture(const TextureDescriptor &descriptor);

    protected:
        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_depth;
        uint32_t m_array_size;
        uint32_t m_mip_levels;
        TextureFormat m_format;
        TextureDimension m_dimension;
        TextureUsage m_usage;
        std::shared_ptr<TextureView> m_view;
    };
} // namespace hyper_rhi
