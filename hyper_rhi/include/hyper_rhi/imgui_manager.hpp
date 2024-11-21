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
        class ICommandList;
        class ISurface;

        class IImGuiManager
        {
        public:
            virtual ~IImGuiManager() = default;

            virtual void initialize(const he::platform::Window &window, const std::shared_ptr<ISurface> &surface) = 0;
            virtual void destroy() = 0;

            virtual void new_frame() = 0;
            virtual void render(const std::shared_ptr<ICommandList> &command_list) = 0;

            virtual void on_resize(uint32_t width, uint32_t height) = 0;
        };
    } // namespace rhi
} // namespace he