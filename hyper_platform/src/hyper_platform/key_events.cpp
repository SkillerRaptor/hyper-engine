/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_platform/key_events.hpp"

#include "hyper_platform/key_codes.hpp"

namespace hyper_engine
{
    KeyPressEvent::KeyPressEvent(const KeyCode key_code)
        : m_key_code(key_code)
    {
    }

    KeyCode KeyPressEvent::key_code() const
    {
        return m_key_code;
    }

    KeyReleaseEvent::KeyReleaseEvent(const KeyCode key_code)
        : m_key_code(key_code)
    {
    }

    KeyCode KeyReleaseEvent::key_code() const
    {
        return m_key_code;
    }
} // namespace hyper_engine