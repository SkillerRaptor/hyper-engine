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

#include <type_traits>

#include "hyper_core/assertion.hpp"
#include "hyper_core/weakable.hpp"

namespace hyper_engine
{
    template <typename T>
    class WeakPtr
    {
        template <typename U>
        friend class Weakable;

    public:
        WeakPtr() = default;

        template <typename U>
        WeakPtr(const WeakPtr<U> &other)
            requires(std::is_base_of_v<T, U>)
            : m_link(other.m_link)
        {
        }

        template <typename U>
        WeakPtr(WeakPtr<U> &&other)
            requires(std::is_base_of_v<T, U>)
            : m_link(other.take_link())
        {
        }

        template <typename U>
        WeakPtr &operator=(WeakPtr<U> &&other)
            requires(std::is_base_of_v<T, U>)
        {
            m_link = other.take_link();
            return *this;
        }

        template <typename U>
        WeakPtr &operator=(WeakPtr<U> const &other)
            requires(std::is_base_of_v<T, U>)
        {
            if (static_cast<void const *>(this) != static_cast<void const *>(&other))
            {
                m_link = other.m_link;
            }

            return *this;
        }

        WeakPtr &operator=(std::nullptr_t)
        {
            clear();
            return *this;
        }

        template <typename U>
        WeakPtr(const U &object)
            requires(std::is_base_of_v<T, U>)
            : m_link(object.template make_weak_ptr<U>().take_link())
        {
        }

        template <typename U>
        WeakPtr(const U *object)
            requires(std::is_base_of_v<T, U>)
        {
            if (object)
            {
                m_link = object->template make_weak_ptr<U>().take_link();
            }
        }

        template <typename U>
        WeakPtr(const RefPtr<U> &object)
            requires(std::is_base_of_v<T, U>)
        {
            if (object)
            {
                m_link = object->template make_weak_ptr<U>().take_link();
            }
        }

        template <typename U>
        WeakPtr(const NonnullRefPtr<U> &object)
            requires(std::is_base_of_v<T, U>)
        {
            m_link = object->template make_weak_ptr<U>().take_link();
        }

        template <typename U>
        WeakPtr &operator=(const U &object)
            requires(std::is_base_of_v<T, U>)
        {
            m_link = object.template make_weak_ptr<U>().take_link();
            return *this;
        }

        template <typename U>
        WeakPtr &operator=(const U *object)
            requires(std::is_base_of_v<T, U>)
        {
            if (object)
            {
                m_link = object->template make_weak_ptr<U>().take_link();
            }
            else
            {
                m_link = nullptr;
            }

            return *this;
        }

        template <typename U>
        WeakPtr &operator=(const RefPtr<U> &object)
            requires(std::is_base_of_v<T, U>)
        {
            if (object)
            {
                m_link = object->template make_weak_ptr<U>().take_link();
            }
            else
            {
                m_link = nullptr;
            }

            return *this;
        }

        template <typename U>
        WeakPtr &operator=(const NonnullRefPtr<U> &object)
            requires(std::is_base_of_v<T, U>)
        {
            m_link = object->template make_weak_ptr<U>().take_link();
            return *this;
        }

        RefPtr<T> strong_ref() const
        {
            return RefPtr<T>(ptr());
        }

        T *ptr() const
        {
            return unsafe_ptr();
        }

        T *operator->() const
        {
            return unsafe_ptr();
        }

        operator T *() const
        {
            return unsafe_ptr();
        }

        T *unsafe_ptr() const
        {
            if (m_link)
            {
                return m_link->template unsafe_ptr<T>();
            }

            return nullptr;
        }

        NonnullRefPtr<T> value() const
        {
            HE_ASSERT(has_value());
            return *unsafe_ptr();
        }

        operator bool() const
        {
            return m_link ? !m_link->is_null() : false;
        }

        bool is_null() const
        {
            return !m_link || m_link->is_null();
        }

        bool has_value() const
        {
            return !is_null();
        }

        void clear()
        {
            m_link = nullptr;
        }

        RefPtr<WeakLink> take_link()
        {
            return std::move(m_link);
        }

    private:
        WeakPtr(const RefPtr<WeakLink> &link)
            : m_link(link)
        {
        }

        RefPtr<WeakLink> m_link;
    };

    template <typename T>
    template <typename U>
    WeakPtr<U> Weakable<T>::make_weak_ptr() const
    {
        if (!m_link)
        {
            m_link = adopt_ref_if_nonnull(new WeakLink(const_cast<T &>(static_cast<T const &>(*this))));
        }

        return WeakPtr<U>(m_link);
    }

    template <typename T>
    WeakPtr<T> make_weak_ptr_if_nonnull(T const *ptr)
    {
        if (ptr)
        {
            return ptr->template try_make_weak_ptr<T>();
        }

        return WeakPtr<T>{};
    }
} // namespace hyper_engine