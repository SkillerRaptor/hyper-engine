/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_rhi/resource.hpp"
#include "hyper_rhi/texture.hpp"

namespace hyper_rhi
{
    enum class TextureComponentSwizzle
    {
        Identity,
        Zero,
        One,
        R,
        G,
        B,
        A,
    };

    struct TextureComponentMapping
    {
        TextureComponentSwizzle r = TextureComponentSwizzle::Identity;
        TextureComponentSwizzle g = TextureComponentSwizzle::Identity;
        TextureComponentSwizzle b = TextureComponentSwizzle::Identity;
        TextureComponentSwizzle a = TextureComponentSwizzle::Identity;
    };

    struct TextureViewDescriptor
    {
        std::string label;

        uint32_t base_mip_level = 0;
        uint32_t mip_level_count = 1;
        uint32_t base_array_level = 0;
        uint32_t array_layer_count = 1;
        TextureComponentMapping component_mapping = {};
        TextureFormat format = TextureFormat::Unknown;
        TextureDimension dimension = TextureDimension::Unknown;
        Texture &texture;
    };

    class TextureView : public Resource
    {
    public:
        virtual ~TextureView() = default;

    protected:
        explicit TextureView(const TextureViewDescriptor &descriptor);

        [[nodiscard]] uint32_t base_mip_level() const;
        [[nodiscard]] uint32_t mip_level_count() const;
        [[nodiscard]] uint32_t base_array_level() const;
        [[nodiscard]] uint32_t array_layer_count() const;
        [[nodiscard]] TextureComponentMapping component_mapping() const;
        [[nodiscard]] TextureFormat format() const;
        [[nodiscard]] TextureDimension dimension() const;

    protected:
        uint32_t m_base_mip_level;
        uint32_t m_mip_level_count;
        uint32_t m_base_array_level;
        uint32_t m_array_layer_count;
        TextureComponentMapping m_component_mapping;
        TextureFormat m_format;
        TextureDimension m_dimension;
    };
} // namespace hyper_rhi