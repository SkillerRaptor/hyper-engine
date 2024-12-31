/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_platform/sdl_event.hpp"

namespace hyper_engine
{
    SdlEvent::SdlEvent(const SDL_Event *event)
        : m_event(event)
    {
    }

    const SDL_Event *SdlEvent::event() const
    {
        return m_event;
    }
} // namespace hyper_engine