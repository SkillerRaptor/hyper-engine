/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

union SDL_Event;

namespace he::platform
{
    class SdlEvent
    {
    public:
        explicit SdlEvent(const SDL_Event *event);

        const SDL_Event *event() const;

    private:
        const SDL_Event *m_event;
    };
} // namespace he::platform