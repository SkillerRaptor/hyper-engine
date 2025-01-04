/*
 * Copyright (c) 2022, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 *
 * Modifications made by SkillerRaptor are licensed under the MIT License.
 */

#pragma once

namespace hyper_engine
{
    template <class T>
    struct DefaultDelete
    {
        constexpr DefaultDelete() = default;

        constexpr void operator()(const T *t)
        {
            delete t;
        }
    };

    template <class T>
    struct DefaultDelete<T[]>
    {
        constexpr DefaultDelete() = default;

        constexpr void operator()(const T *t)
        {
            delete[] t;
        }
    };
} // namespace hyper_engine
