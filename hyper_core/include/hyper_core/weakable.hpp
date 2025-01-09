/*
 * Copyright (c) 2018-2022, Andreas Kling <kling@serenityos.org>
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

#include <type_traits>

#include "hyper_core/ref_counted.hpp"
#include "hyper_core/ref_ptr.hpp"

namespace hyper_engine
{
    template <typename T>
    class Weakable;

    template <typename T>
    class WeakPtr;

    class WeakLink : public RefCounted<WeakLink>
    {
    private:
        template <typename T>
        friend class Weakable;

        template <typename T>
        friend class WeakPtr;

    public:
        template <typename T>
        RefPtr<T> strong_ref() const
            requires(std::is_base_of_v<RefCountedBase, T>)
        {
            return static_cast<T *>(m_ptr);
        }

        template <typename T>
        T *unsafe_ptr() const
        {
            return static_cast<T *>(m_ptr);
        }

        bool is_null() const
        {
            return m_ptr == nullptr;
        }

        void revoke()
        {
            m_ptr = nullptr;
        }

    private:
        template <typename T>
        explicit WeakLink(T &weakable)
            : m_ptr(&weakable)
        {
        }

        mutable void *m_ptr = nullptr;
    };

    template <typename T>
    class Weakable
    {
    private:
        class Link;

    public:
        template <typename U = T>
        WeakPtr<U> make_weak_ptr() const;

    protected:
        Weakable() = default;

        ~Weakable()
        {
            revoke_weak_ptrs();
        }

        void revoke_weak_ptrs()
        {
            if (const RefPtr<WeakLink> link = std::move(m_link))
            {
                link->revoke();
            }
        }

    private:
        mutable RefPtr<WeakLink> m_link = nullptr;
    };
} // namespace hyper_engine
