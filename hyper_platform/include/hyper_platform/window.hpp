/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string_view>

#include <hyper_core/prerequisites.hpp>
#include <hyper_event/event_bus.hpp>

struct SDL_Window;

namespace hyper_platform
{
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

        [[nodiscard]] uint32_t width() const;
        [[nodiscard]] uint32_t height() const;

        [[nodiscard]] HE_FORCE_INLINE SDL_Window *native_window() const
        {
            return m_native_window;
        }

        static void process_events(hyper_event::EventBus &event_bus);
        static void wait_events();

    private:
        SDL_Window *m_native_window;
    };
} // namespace hyper_platform
