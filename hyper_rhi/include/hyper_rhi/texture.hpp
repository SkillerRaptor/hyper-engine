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

namespace hyper_engine
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

        virtual std::string_view label() const = 0;

        virtual uint32_t width() const = 0;
        virtual uint32_t height() const = 0;
        virtual uint32_t depth() const = 0;
        virtual uint32_t array_size() const = 0;
        virtual uint32_t mip_levels() const = 0;
        virtual Format format() const = 0;
        virtual Dimension dimension() const = 0;
        virtual TextureUsage usage() const = 0;
    };
} // namespace hyper_engine
