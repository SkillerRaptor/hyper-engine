/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <hyper_core/prerequisites.hpp>

#include <cstdint>

namespace hyper_rhi
{
    class ResourceHandle
    {
    public:
        ResourceHandle()
            : m_handle(0xffffffff)
        {
        }

        explicit ResourceHandle(const uint32_t handle)
            : m_handle(handle)
        {
        }

        [[nodiscard]] HE_FORCE_INLINE bool valid() const
        {
            return m_handle != 0xffffffff;
        }

        [[nodiscard]] HE_FORCE_INLINE uint32_t handle() const
        {
            return m_handle;
        }

    private:
        uint32_t m_handle;
    };
} // namespace hyper_rhi