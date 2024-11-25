/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string_view>

struct SDL_Window;

namespace hyper_engine
{
    class EventBus;

    struct WindowDescriptor
    {
        std::string_view title;
        uint32_t width;
        uint32_t height;
    };

    class Window
    {
    public:
        explicit Window(const WindowDescriptor &descriptor);
        ~Window();

        void process_events();
        void wait_events();

        uint32_t width() const;
        uint32_t height() const;
        SDL_Window *native_window() const;

    private:
        SDL_Window *m_native_window;
    };
} // namespace hyper_engine
