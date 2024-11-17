/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

#include "hyper_rhi/format.hpp"

namespace he::rhi
{
    class Texture;
    class TextureView;

    class Surface
    {
    public:
        virtual ~Surface() = default;

        virtual uint32_t min_image_count() const = 0;
        virtual uint32_t image_count() const = 0;

        virtual Format format() const = 0;

        virtual std::shared_ptr<Texture> current_texture() const = 0;
        virtual std::shared_ptr<TextureView> current_texture_view() const = 0;

        void resize(const uint32_t width, const uint32_t height);

        bool resized() const;
        uint32_t width() const;
        uint32_t height() const;

    protected:
        Surface(const uint32_t width, const uint32_t height);

    protected:
        bool m_resized;
        uint32_t m_width;
        uint32_t m_height;
    };
} // namespace he::rhi
