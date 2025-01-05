/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_platform/key_codes.hpp"

namespace hyper_engine
{
    class KeyPressEvent
    {
    public:
        explicit KeyPressEvent(KeyCode);

        KeyCode key_code() const;

    private:
        KeyCode m_key_code = KeyCode::Unknown;
    };

    class KeyReleaseEvent
    {
    public:
        explicit KeyReleaseEvent(KeyCode);

        KeyCode key_code() const;

    private:
        KeyCode m_key_code = KeyCode::Unknown;
    };
} // namespace hyper_engine
