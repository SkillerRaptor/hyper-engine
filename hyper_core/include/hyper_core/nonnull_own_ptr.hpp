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

#include <algorithm>
#include <type_traits>
#include <utility>

#include "hyper_core/assertion.hpp"
#include "hyper_core/forward.hpp"
#include "hyper_core/ref_counted.hpp"

namespace hyper_engine
{
    template <typename T>
    class NonnullOwnPtr
    {
    public:
        using ElementType = T;

        enum AdoptTag
        {
            Adopt
        };

    public:
        NonnullOwnPtr(AdoptTag, T &ptr)
            : m_ptr(&ptr)
        {
            static_assert(
                requires { requires typename T::AllowOwnPtr()(); } ||
                    !requires {
                        requires !typename T::AllowOwnPtr()();
                        std::declval<T>().ref();
                        std::declval<T>().unref();
                    },
                "Use NonnullRefPtr<T> for RefCounted types");
        }

        NonnullOwnPtr(NonnullOwnPtr &&other)
            : m_ptr(other.leak_ptr())
        {
            HE_ASSERT(m_ptr);
        }

        template <typename U>
        NonnullOwnPtr(NonnullOwnPtr<U> &&other)
            : m_ptr(other.leak_ptr())
        {
            HE_ASSERT(m_ptr);
        }

        ~NonnullOwnPtr()
        {
            clear();
        }

        NonnullOwnPtr(const NonnullOwnPtr &) = delete;
        NonnullOwnPtr &operator=(const NonnullOwnPtr &) = delete;

        template <typename U>
        NonnullOwnPtr(const NonnullOwnPtr<U> &) = delete;
        template <typename U>
        NonnullOwnPtr &operator=(const NonnullOwnPtr<U> &) = delete;

        template <typename U>
        NonnullOwnPtr(const RefPtr<U> &) = delete;
        template <typename U>
        NonnullOwnPtr &operator=(const RefPtr<U> &) = delete;

        template <typename U>
        NonnullOwnPtr(const NonnullRefPtr<U> &) = delete;
        template <typename U>
        NonnullOwnPtr &operator=(const NonnullRefPtr<U> &) = delete;

        NonnullOwnPtr &operator=(NonnullOwnPtr &&other) noexcept
        {
            NonnullOwnPtr ptr(std::move(other));
            swap(ptr);
            return *this;
        }

        template <typename U>
        NonnullOwnPtr &operator=(NonnullOwnPtr<U> &&other)
        {
            NonnullOwnPtr ptr(std::move(other));
            swap(ptr);
            return *this;
        }

        T *leak_ptr()
        {
            return std::exchange(m_ptr, nullptr);
        }

        T *ptr() const
        {
            HE_ASSERT(m_ptr);
            return m_ptr;
        }

        T *operator->() const
        {
            return ptr();
        }

        T &operator*() const
        {
            return *ptr();
        }

        operator T *() const
        {
            return ptr();
        }

        operator bool() const = delete;
        bool operator!() const = delete;

        void swap(NonnullOwnPtr &other)
        {
            using std::swap;

            swap(m_ptr, other.m_ptr);
        }

        template <typename U>
        void swap(NonnullOwnPtr<U> &other)
        {
            using std::swap;

            swap(m_ptr, other.m_ptr);
        }

        template <typename U>
        NonnullOwnPtr<U> release_nonnull()
        {
            HE_ASSERT(m_ptr);
            return NonnullOwnPtr<U>(NonnullOwnPtr<U>::Adopt, static_cast<U &>(*leak_ptr()));
        }

    private:
        void clear()
        {
            const T *ptr = std::exchange(m_ptr, nullptr);
            delete ptr;
        }

    private:
        T *m_ptr = nullptr;
    };

    template <typename T>
    NonnullOwnPtr<T> adopt_own(T &object)
    {
        return NonnullOwnPtr(NonnullOwnPtr<T>::Adopt, object);
    }

    template <class T, class... Args>
        requires(std::is_constructible_v<T, Args...>)
    NonnullOwnPtr<T> make(Args &&...args)
    {
        return NonnullOwnPtr(NonnullOwnPtr<T>::Adopt, *new T(std::forward<Args>(args)...));
    }

    template <typename T, typename U>
    void swap(NonnullOwnPtr<T> &a, NonnullOwnPtr<U> &b)
    {
        a.swap(b);
    }
} // namespace hyper_engine
