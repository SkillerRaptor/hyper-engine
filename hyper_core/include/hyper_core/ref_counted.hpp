/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
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

#include "hyper_core/assertion.hpp"

namespace hyper_engine
{
    class RefCountedBase
    {
    public:
        using AllowOwnPtr = std::false_type;

    public:
        RefCountedBase(RefCountedBase const &) = delete;
        RefCountedBase &operator=(RefCountedBase const &) = delete;

        RefCountedBase(RefCountedBase &&) = delete;
        RefCountedBase &operator=(RefCountedBase &&) = delete;

        void ref() const;

    protected:
        RefCountedBase();
        ~RefCountedBase();

        bool deref_base() const;

    private:
        mutable uint32_t m_ref_count;
    };

    template <typename T>
    class RefCounted : public RefCountedBase
    {
    public:
        bool unref() const
        {
            if (deref_base())
            {
                delete static_cast<T const *>(this);
                return true;
            }

            return false;
        }
    };
} // namespace hyper_engine
