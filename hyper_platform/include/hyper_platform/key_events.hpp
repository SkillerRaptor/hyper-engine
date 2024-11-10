/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/prerequisites.hpp>

#include "hyper_platform/key_codes.hpp"

namespace hyper_platform
{
    class KeyPressEvent
    {
    public:
        explicit KeyPressEvent(const KeyCode key_code)
            : m_key_code(key_code)
        {
        }

        [[nodiscard]] HE_FORCE_INLINE KeyCode key_code() const
        {
            return m_key_code;
        }

    private:
        KeyCode m_key_code;
    };

    class KeyReleaseEvent
    {
    public:
        explicit KeyReleaseEvent(const KeyCode key_code)
            : m_key_code(key_code)
        {
        }

        [[nodiscard]] HE_FORCE_INLINE KeyCode key_code() const
        {
            return m_key_code;
        }

    private:
        KeyCode m_key_code;
    };
} // namespace hyper_platform
