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
#include "hyper_core/nonnull_ref_ptr.hpp"

namespace hyper_engine
{
    template <typename T>
    class RefPtr
    {
    private:
        template <typename U>
        friend class RefPtr;

        template <typename U>
        friend class NonnullRefPtr;

    public:
        enum AdoptTag
        {
            Adopt
        };

        RefPtr() = default;
        RefPtr(const T *ptr)
            : m_ptr(const_cast<T *>(ptr))
        {
            ref_if_not_null(m_ptr);
        }

        RefPtr(const T &object)
            : m_ptr(const_cast<T *>(&object))
        {
            m_ptr->ref();
        }

        RefPtr(AdoptTag, T &object)
            : m_ptr(&object)
        {
        }

        RefPtr(RefPtr &&other) noexcept
            : m_ptr(other.leak_ref())
        {
        }

        RefPtr(const NonnullRefPtr<T> &other)
            : m_ptr(const_cast<T *>(other.ptr()))
        {
            m_ptr->ref();
        }

        template <typename U>
            requires(std::is_convertible_v<U *, T *>)
        RefPtr(const NonnullRefPtr<U> &other)
            : m_ptr(const_cast<T *>(static_cast<const T *>(other.ptr())))
        {
            m_ptr->ref();
        }

        template <typename U>
            requires(std::is_convertible_v<U *, T *>)
        RefPtr(NonnullRefPtr<U> &&other)
            : m_ptr(static_cast<T *>(&other.leak_ref()))
        {
        }

        template <typename U>
            requires(std::is_convertible_v<U *, T *>)
        RefPtr(RefPtr<U> &&other)
            : m_ptr(static_cast<T *>(other.leak_ref()))
        {
        }

        RefPtr(const RefPtr &other)
            : m_ptr(other.m_ptr)
        {
            ref_if_not_null(m_ptr);
        }

        template <typename U>
        RefPtr(const RefPtr<U> &other)
            requires(std::is_convertible_v<U *, T *>)
            : m_ptr(const_cast<T *>(static_cast<const T *>(other.ptr())))
        {
            ref_if_not_null(m_ptr);
        }

        ~RefPtr()
        {
            clear();
        }

        template <typename U>
        RefPtr(const OwnPtr<U> &) = delete;
        template <typename U>
        RefPtr &operator=(const OwnPtr<U> &) = delete;

        void swap(RefPtr &other)
        {
            using std::swap;

            swap(m_ptr, other.m_ptr);
        }

        template <typename U>
        void swap(RefPtr<U> &other)
            requires(std::is_convertible_v<U *, T *>)
        {
            using std::swap;

            swap(m_ptr, other.m_ptr);
        }

        RefPtr &operator=(RefPtr &&other) noexcept
        {
            RefPtr tmp{std::move(other)};
            swap(tmp);
            return *this;
        }

        template <typename U>
            requires(std::is_convertible_v<U *, T *>)
        RefPtr &operator=(RefPtr<U> &&other)
        {
            RefPtr tmp{std::move(other)};
            swap(tmp);
            return *this;
        }

        template <typename U>
            requires(std::is_convertible_v<U *, T *>)
        RefPtr &operator=(NonnullRefPtr<U> &&other)
        {
            RefPtr tmp{std::move(other)};
            swap(tmp);
            return *this;
        }

        RefPtr &operator=(const NonnullRefPtr<T> &other)
        {
            RefPtr tmp{other};
            swap(tmp);
            return *this;
        }

        template <typename U>
            requires(std::is_convertible_v<U *, T *>)
        RefPtr &operator=(const NonnullRefPtr<U> &other)
        {
            RefPtr tmp{other};
            swap(tmp);
            return *this;
        }

        RefPtr &operator=(const RefPtr &other)
        {
            RefPtr tmp{other};
            swap(tmp);
            return *this;
        }

        template <typename U>
            requires(std::is_convertible_v<U *, T *>)
        RefPtr &operator=(const RefPtr<U> &other)
        {
            RefPtr tmp{other};
            swap(tmp);
            return *this;
        }

        RefPtr &operator=(const T *ptr)
        {
            RefPtr tmp{ptr};
            swap(tmp);
            return *this;
        }

        RefPtr &operator=(const T &object)
        {
            RefPtr tmp{object};
            swap(tmp);
            return *this;
        }

        RefPtr &operator=(std::nullptr_t)
        {
            clear();
            return *this;
        }

        bool assign_if_null(RefPtr &&other)
        {
            if (this == &other)
            {
                return is_null();
            }

            *this = std::move(other);
            return true;
        }

        template <typename U>
        bool assign_if_null(RefPtr<U> &&other)
        {
            if (this == &other)
            {
                return is_null();
            }

            *this = std::move(other);
            return true;
        }

        void clear()
        {
            T *ptr = std::exchange(m_ptr, nullptr);
            unref_if_not_null(ptr);
        }

        bool operator!() const
        {
            return !m_ptr;
        }

        T *leak_ref()
        {
            return std::exchange(m_ptr, nullptr);
        }

        NonnullRefPtr<T> release_nonnull()
        {
            T *ptr = leak_ref();
            HE_ASSERT(ptr);
            return NonnullRefPtr<T>(NonnullRefPtr<T>::Adopt, *ptr);
        }

        T *ptr() const
        {
            return as_ptr();
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
            return as_ptr();
        }

        operator bool()
        {
            return !is_null();
        }

        bool operator==(std::nullptr_t) const
        {
            return is_null();
        }

        bool operator==(const RefPtr &other) const
        {
            return as_ptr() == other.as_ptr();
        }

        template <typename U>
        bool operator==(const NonnullRefPtr<U> &other) const
        {
            return as_ptr() == other.m_ptr;
        }

        template <typename RawPtr>
            requires(std::is_pointer_v<RawPtr>)
        bool operator==(RawPtr other) const
        {
            return as_ptr() == other;
        }

        bool is_null() const
        {
            return !m_ptr;
        }

    private:
        T *as_ptr() const
        {
            return m_ptr;
        }

        T *as_nonnull_ptr() const
        {
            HE_ASSERT(m_ptr);
            return m_ptr;
        }

        T *m_ptr{nullptr};
    };

    template <typename T, typename U>
    NonnullRefPtr<T> static_ptr_cast(const NonnullRefPtr<U> &ptr)
    {
        return NonnullRefPtr<T>(static_cast<const T &>(*ptr));
    }

    template <typename T, typename U>
    RefPtr<T> static_ptr_cast(const RefPtr<U> &ptr)
    {
        return RefPtr<T>(static_cast<const T *>(ptr.ptr()));
    }

    template <typename T, typename U>
        requires(std::is_convertible_v<U *, T *>)
    void swap(RefPtr<T> &a, RefPtr<U> &b)
    {
        a.swap(b);
    }

    template <typename T>
    RefPtr<T> adopt_ref_if_nonnull(T *object)
    {
        if (object)
        {
            return RefPtr<T>(RefPtr<T>::Adopt, *object);
        }

        return {};
    }

} // namespace hyper_engine
