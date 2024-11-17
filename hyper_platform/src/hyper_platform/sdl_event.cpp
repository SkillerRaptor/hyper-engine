/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_platform/sdl_event.hpp"

namespace he::platform
{
    SdlEvent::SdlEvent(const SDL_Event *event)
        : m_event(event)
    {
    }

    const SDL_Event *SdlEvent::event() const
    {
        return m_event;
    }
} // namespace he::platform