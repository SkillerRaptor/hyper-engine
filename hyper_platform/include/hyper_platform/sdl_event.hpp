/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/prerequisites.hpp>

#include <SDL3/SDL.h>

namespace hyper_platform
{

    class SdlEvent
    {
    public:
        explicit SdlEvent(const SDL_Event &event)
            : m_event(event)
        {
        }

        [[nodiscard]] HE_FORCE_INLINE const SDL_Event *event() const
        {
            return &m_event;
        }

    private:
        SDL_Event m_event;
    };
} // namespace hyper_platform