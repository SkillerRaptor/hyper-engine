/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string_view>

struct SDL_Window;

namespace hyper_engine
{
    struct WindowDescriptor
    {
        std::string_view title;
        uint32_t width = 1280;
        uint32_t height = 720;
    };

    class Window
    {
    public:
        explicit Window(const WindowDescriptor &);
        ~Window();

        void process_events();
        void wait_events();

        uint32_t width() const;
        uint32_t height() const;
        SDL_Window *native_window() const;

    private:
        SDL_Window *m_native_window = nullptr;
    };
} // namespace hyper_engine
