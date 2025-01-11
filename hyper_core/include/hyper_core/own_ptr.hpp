/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <memory>

namespace hyper_engine
{
    template <typename T>
    using OwnPtr = std::unique_ptr<T>;

    template <typename T, typename... Args>
    OwnPtr<T> make_own(Args &&...args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
} // namespace hyper_engine