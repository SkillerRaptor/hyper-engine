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

        uint32_t width() const;
        uint32_t height() const;
        SDL_Window *native_window() const;

        static void process_events(EventBus &event_bus);
        static void wait_events();

    private:
        SDL_Window *m_native_window;
    };
} // namespace hyper_engine
