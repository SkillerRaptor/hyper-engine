/*
 * Copyright (c) 2018-2023, Andreas Kling <kling@serenityos.org>
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
#include <utility>

#include "hyper_core/assertion.hpp"
#include "hyper_core/forward.hpp"

namespace hyper_engine
{
    template <typename T>
    class RefPtr;

    template <typename T>
    void ref_if_not_null(T *ptr)
    {
        if (ptr)
        {
            ptr->ref();
        }
    }

    template <typename T>
    void unref_if_not_null(T *ptr)
    {
        if (ptr)
        {
            ptr->unref();
        }
    }

    template <typename T>
    class NonnullRefPtr
    {
    private:
        template <typename U>
        friend class RefPtr;

        template <typename U>
        friend class NonnullRefPtr;

    public:
        using ElementType = T;

        enum AdoptTag
        {
            Adopt
        };

    public:
        NonnullRefPtr(const T &object)
            : m_ptr(const_cast<T *>(&object))
        {
            m_ptr->ref();
        }

        template <typename U>
            requires(std::is_convertible_v<U *, T *>)
        NonnullRefPtr(const U &object)
            : m_ptr(const_cast<T *>(static_cast<T const *>(&object)))
        {
            m_ptr->ref();
        }

        NonnullRefPtr(AdoptTag, T &object)
            : m_ptr(&object)
        {
        }

        NonnullRefPtr(NonnullRefPtr &&other) noexcept
            : m_ptr(&other.leak_ref())
        {
        }

        template <typename U>
            requires(std::is_convertible_v<U *, T *>)
        NonnullRefPtr(NonnullRefPtr<U> &&other)
            : m_ptr(static_cast<T *>(&other.leak_ref()))
        {
        }

        NonnullRefPtr(const NonnullRefPtr &other)
            : m_ptr(const_cast<T *>(other.ptr()))
        {
            m_ptr->ref();
        }

        template <typename U>
            requires(std::is_convertible_v<U *, T *>)
        NonnullRefPtr(const NonnullRefPtr<U> &other)
            : m_ptr(const_cast<T *>(static_cast<const T *>(other.ptr())))
        {
            m_ptr->ref();
        }

        NonnullRefPtr() = delete;

        ~NonnullRefPtr()
        {
            T *ptr = std::exchange(m_ptr, nullptr);
            unref_if_not_null(ptr);
        }

        template <typename U>
        NonnullRefPtr(OwnPtr<U> const &) = delete;
        template <typename U>
        NonnullRefPtr &operator=(OwnPtr<U> const &) = delete;

        NonnullRefPtr(RefPtr<T> const &) = delete;
        NonnullRefPtr &operator=(RefPtr<T> const &) = delete;

        template <typename U>
        NonnullRefPtr(RefPtr<U> const &) = delete;
        template <typename U>
        NonnullRefPtr &operator=(RefPtr<U> const &) = delete;

        NonnullRefPtr &operator=(const NonnullRefPtr &other)
        {
            NonnullRefPtr tmp = other;
            swap(tmp);
            return *this;
        }

        template <typename U>
            requires(std::is_convertible_v<U *, T *>)
        NonnullRefPtr &operator=(const NonnullRefPtr<U> &other)
        {
            NonnullRefPtr tmp = other;
            swap(tmp);
            return *this;
        }

        NonnullRefPtr &operator=(NonnullRefPtr &&other) noexcept
        {
            NonnullRefPtr tmp = std::move(other);
            swap(tmp);
            return *this;
        }

        template <typename U>
            requires(std::is_convertible_v<U *, T *>)
        NonnullRefPtr &operator=(NonnullRefPtr<U> &&other)
        {
            NonnullRefPtr tmp = std::move(other);
            swap(tmp);
            return *this;
        }

        NonnullRefPtr &operator=(const T &object)
        {
            NonnullRefPtr tmp = object;
            swap(tmp);
            return *this;
        }

        T &leak_ref()
        {
            T *ptr = std::exchange(m_ptr, nullptr);
            HE_ASSERT(ptr);
            return *ptr;
        }

        T *ptr() const
        {
            return as_nonnull_ptr();
        }

        T *operator->() const
        {
            return as_nonnull_ptr();
        }

        T &operator*() const
        {
            return *as_nonnull_ptr();
        }

        operator T *() const
        {
            return as_nonnull_ptr();
        }

        operator T &() const
        {
            return *as_nonnull_ptr();
        }

        operator bool() const = delete;
        bool operator!() const = delete;

        void swap(NonnullRefPtr &other)
        {
            using std::swap;

            swap(m_ptr, other.m_ptr);
        }

        template <typename U>
        void swap(NonnullRefPtr<U> &other)
            requires(std::is_convertible_v<U *, T *>)
        {
            using std::swap;

            swap(m_ptr, other.m_ptr);
        }

        bool operator==(const NonnullRefPtr &other) const
        {
            return m_ptr == other.m_ptr;
        }

        template <typename RawPtr>
            requires(std::is_pointer_v<RawPtr>)
        bool operator==(RawPtr other) const
        {
            return m_ptr == other;
        }

    private:
        T *as_nonnull_ptr() const
        {
            HE_ASSERT(m_ptr);
            return m_ptr;
        }

    private:
        T *m_ptr = nullptr;
    };

    template <typename T>
    NonnullRefPtr<T> adopt_ref(T &object)
    {
        return NonnullRefPtr(NonnullRefPtr<T>::Adopt, object);
    }

    template <typename T, class... Args>
        requires(std::is_constructible_v<T, Args...>)
    NonnullRefPtr<T> make_ref_counted(Args &&...args)
    {
        return NonnullRefPtr(NonnullRefPtr<T>::Adopt, *new T(std::forward<Args>(args)...));
    }

    template <typename T, typename U>
        requires(std::is_convertible_v<U *, T *>)
    void swap(NonnullRefPtr<T> &a, NonnullRefPtr<U> &b)
    {
        a.swap(b);
    }
} // namespace hyper_engine
