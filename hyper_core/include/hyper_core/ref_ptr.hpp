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
    using RefPtr = std::shared_ptr<T>;

    template <typename T>
    using WeakPtr = std::weak_ptr<T>;

    template <typename T, typename... Args>
    RefPtr<T> make_ref(Args &&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
} // namespace hyper_engine