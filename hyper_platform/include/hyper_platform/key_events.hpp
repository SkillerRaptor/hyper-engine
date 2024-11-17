/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_platform/key_codes.hpp"

namespace he::platform
{
    class KeyPressEvent
    {
    public:
        explicit KeyPressEvent(KeyCode key_code);

        KeyCode key_code() const;

    private:
        KeyCode m_key_code;
    };

    class KeyReleaseEvent
    {
    public:
        explicit KeyReleaseEvent(KeyCode key_code);

        KeyCode key_code() const;

    private:
        KeyCode m_key_code;
    };
} // namespace he::platform
