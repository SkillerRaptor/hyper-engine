/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "hyper_core/default_delete.hpp"

namespace hyper_engine
{
    template <typename T>
    class NonnullRefPtr;

    template <typename T>
    class NonnullOwnPtr;

    template <typename T>
    class RefPtr;

    template <typename T, typename Deleter = DefaultDelete<T>>
    class OwnPtr;
} // namespace hyper_engine