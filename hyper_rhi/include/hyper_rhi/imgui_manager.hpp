/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

#include <hyper_platform/window.hpp>

#include "hyper_rhi/command_list.hpp"
#include "hyper_rhi/surface.hpp"

namespace hyper_rhi
{
    class ImGuiManager
    {
    public:
        virtual ~ImGuiManager() = default;

        virtual void initialize(const hyper_platform::Window &window, const std::shared_ptr<Surface> &surface) = 0;
        virtual void destroy() = 0;

        virtual void new_frame() = 0;
        virtual void render(const std::shared_ptr<CommandList> &command_list) = 0;

        virtual void on_resize(uint32_t width, uint32_t height) = 0;
    };
} // namespace hyper_rhi