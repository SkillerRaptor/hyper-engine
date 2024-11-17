/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>

#include <hyper_core/bits.hpp>

#include "hyper_rhi/dimension.hpp"
#include "hyper_rhi/format.hpp"

namespace he::rhi
{
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
        Dimension dimension = Dimension::Unknown;
        TextureUsage usage = TextureUsage::None;
    };

    class Texture
    {
    public:
        virtual ~Texture() = default;

        std::string_view label() const;

        uint32_t width() const;
        uint32_t height() const;
        uint32_t depth() const;
        uint32_t array_size() const;
        uint32_t mip_levels() const;
        Format format() const;
        Dimension dimension() const;
        TextureUsage usage() const;

    protected:
        explicit Texture(const TextureDescriptor &descriptor);

    protected:
        std::string m_label;

        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_depth;
        uint32_t m_array_size;
        uint32_t m_mip_levels;
        Format m_format;
        Dimension m_dimension;
        TextureUsage m_usage;
    };
} // namespace he::rhi
