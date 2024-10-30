/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint>

namespace hyper_rhi
{
    class ResourceHandle
    {
    public:
        ResourceHandle();
        explicit ResourceHandle(uint32_t handle);

        [[nodiscard]] bool is_valid() const;

        [[nodiscard]] uint32_t handle() const;

    private:
        uint32_t m_handle;
    };
} // namespace hyper_rhi