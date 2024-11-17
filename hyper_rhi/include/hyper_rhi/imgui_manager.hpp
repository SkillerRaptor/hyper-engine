/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

namespace he
{
    namespace platform
    {
        class Window;
    } // namespace platform

    namespace rhi
    {
        class CommandList;
        class Surface;

        class ImGuiManager
        {
        public:
            virtual ~ImGuiManager() = default;

            virtual void initialize(const he::platform::Window &window, const std::shared_ptr<Surface> &surface) = 0;
            virtual void destroy() = 0;

            virtual void new_frame() = 0;
            virtual void render(const std::shared_ptr<CommandList> &command_list) = 0;

            virtual void on_resize(uint32_t width, uint32_t height) = 0;
        };
    } // namespace rhi
} // namespace he