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

#include <utility>

#include "hyper_core/assertion.hpp"
#include "hyper_core/forward.hpp"
#include "hyper_core/nonnull_own_ptr.hpp"

namespace hyper_engine
{
    template <typename T, typename Deleter>
    class OwnPtr
    {
    public:
        OwnPtr() = default;

        OwnPtr(std::nullptr_t)
            : m_ptr(nullptr)
        {
        }

        OwnPtr(OwnPtr &&other) noexcept
            : m_ptr(other.leak_ptr())
        {
        }

        template <typename U>
        OwnPtr(NonnullOwnPtr<U> &&other)
            : m_ptr(other.leak_ptr())
        {
        }

        template <typename U>
        OwnPtr(OwnPtr<U> &&other)
            : m_ptr(other.leak_ptr())
        {
        }

        ~OwnPtr()
        {
            clear();
        }

        OwnPtr(const OwnPtr &) = delete;
        OwnPtr &operator=(const OwnPtr &) = delete;

        template <typename U>
        OwnPtr(const OwnPtr<U> &) = delete;
        template <typename U>
        OwnPtr &operator=(const OwnPtr<U> &) = delete;

        template <typename U>
        OwnPtr(const NonnullOwnPtr<U> &) = delete;
        template <typename U>
        OwnPtr &operator=(const NonnullOwnPtr<U> &) = delete;

        template <typename U>
        OwnPtr(const RefPtr<U> &) = delete;
        template <typename U>
        OwnPtr &operator=(const RefPtr<U> &) = delete;

        template <typename U>
        OwnPtr(const NonnullRefPtr<U> &) = delete;
        template <typename U>
        OwnPtr &operator=(const NonnullRefPtr<U> &) = delete;

        template <typename U>
        OwnPtr(WeakPtr<U> const &) = delete;
        template <typename U>
        OwnPtr &operator=(WeakPtr<U> const &) = delete;

        OwnPtr &operator=(OwnPtr &&other) noexcept
        {
            OwnPtr ptr = std::move(other);
            swap(ptr);
            return *this;
        }

        template <typename U>
        OwnPtr &operator=(OwnPtr<U> &&other)
        {
            OwnPtr ptr = std::move(other);
            swap(ptr);
            return *this;
        }

        template <typename U>
        OwnPtr &operator=(NonnullOwnPtr<U> &&other)
        {
            OwnPtr ptr = std::move(other);
            swap(ptr);
            HE_ASSERT(m_ptr);
            return *this;
        }

        OwnPtr &operator=(T *ptr) = delete;

        OwnPtr &operator=(std::nullptr_t)
        {
            clear();
            return *this;
        }

        void clear()
        {
            T *ptr = std::exchange(m_ptr, nullptr);
            Deleter{}(ptr);
        }

        bool operator!() const
        {
            return !m_ptr;
        }

        T *leak_ptr()
        {
            T *leaked_ptr = m_ptr;
            m_ptr = nullptr;
            return leaked_ptr;
        }

        NonnullOwnPtr<T> release_nonnull()
        {
            HE_ASSERT(m_ptr);
            return NonnullOwnPtr(NonnullOwnPtr<T>::Adopt, *leak_ptr());
        }

        template <typename U>
        NonnullOwnPtr<U> release_nonnull()
        {
            HE_ASSERT(m_ptr);
            return NonnullOwnPtr<U>(NonnullOwnPtr<U>::Adopt, static_cast<U &>(*leak_ptr()));
        }

        T *ptr() const
        {
            return m_ptr;
        }

        T *operator->() const
        {
            HE_ASSERT(m_ptr);
            return m_ptr;
        }

        T &operator*() const
        {
            HE_ASSERT(m_ptr);
            return *m_ptr;
        }

        operator T *() const
        {
            return m_ptr;
        }

        operator bool()
        {
            return !!m_ptr;
        }

        void swap(OwnPtr &other)
        {
            using std::swap;

            swap(m_ptr, other.m_ptr);
        }

        template <typename U>
        void swap(OwnPtr<U> &other)
        {
            using std::swap;

            swap(m_ptr, other.m_ptr);
        }

        static OwnPtr lift(T *ptr)
        {
            return {ptr};
        }

    protected:
        explicit OwnPtr(T *ptr)
            : m_ptr(ptr)
        {
            static_assert(
                requires { requires typename T::AllowOwnPtr()(); } ||
                    !requires {
                        requires !typename T::AllowOwnPtr()();
                        std::declval<T>().ref();
                        std::declval<T>().unref();
                    },
                "Use RefPtr<> for RefCounted types");
        }

    private:
        T *m_ptr = nullptr;
    };

    template <typename T>
    OwnPtr<T> adopt_own_if_nonnull(T *object)
    {
        if (object)
        {
            return OwnPtr<T>::lift(object);
        }

        return {};
    }

    template <typename T, typename U>
    void swap(OwnPtr<T> &a, OwnPtr<U> &b)
    {
        a.swap(b);
    }
} // namespace hyper_engine
