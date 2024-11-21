/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint>

namespace hyper_engine
{
    class ResourceHandle
    {
    public:
        ResourceHandle();
        explicit ResourceHandle(const uint32_t handle);

        bool valid() const;
        uint32_t handle() const;

    private:
        uint32_t m_handle;
    };
} // namespace hyper_engine