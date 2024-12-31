/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

#include "hyper_rhi/format.hpp"

namespace hyper_engine
{
    class Texture;
    class TextureView;

    class Surface
    {
    public:
        virtual ~Surface() = default;

        virtual void resize(uint32_t width, uint32_t height) = 0;

        virtual bool resized() const = 0;
        virtual uint32_t width() const = 0;
        virtual uint32_t height() const = 0;

        virtual uint32_t min_image_count() const = 0;
        virtual uint32_t image_count() const = 0;

        virtual Format format() const = 0;

        virtual std::shared_ptr<Texture> current_texture() const = 0;
        virtual std::shared_ptr<TextureView> current_texture_view() const = 0;
    };
} // namespace hyper_engine
