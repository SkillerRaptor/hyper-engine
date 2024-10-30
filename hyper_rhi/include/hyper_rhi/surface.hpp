/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

#include <hyper_platform/window.hpp>

#include "hyper_rhi/texture.hpp"

namespace hyper_rhi
{
    class Surface
    {
    public:
        virtual ~Surface() = default;

        void resize(uint32_t width, uint32_t height);

        [[nodiscard]] uint32_t width() const;
        [[nodiscard]] uint32_t height() const;
        [[nodiscard]] virtual TextureHandle current_texture() const = 0;

    protected:
        Surface(uint32_t width, uint32_t height);

    protected:
        bool m_resized;
        uint32_t m_width;
        uint32_t m_height;
    };

    using SurfaceHandle = std::shared_ptr<Surface>;
} // namespace hyper_rhi
