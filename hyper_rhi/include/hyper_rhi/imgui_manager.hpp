/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

namespace hyper_engine
{
    class ICommandList;
    class ISurface;
    class Window;

    class IImGuiManager
    {
    public:
        virtual ~IImGuiManager() = default;

        virtual void initialize(const Window &window, const std::shared_ptr<ISurface> &surface) = 0;
        virtual void destroy() = 0;

        virtual void new_frame() = 0;
        virtual void render(const std::shared_ptr<ICommandList> &command_list) = 0;

        virtual void on_resize(uint32_t width, uint32_t height) = 0;
    };
} // namespace hyper_engine