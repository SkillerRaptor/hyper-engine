/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <functional>
#include <vector>

namespace he::event
{
    class IEventHandler
    {
    public:
        virtual ~IEventHandler() = default;
    };

    template <typename T>
    class EventHandler final : public IEventHandler
    {
    public:
        void subscribe(const std::function<void(const T &)> &callback)
        {
            m_callbacks.push_back(callback);
        }

        void dispatch(const T &event)
        {
            for (const std::function<void(const T &)> &callback : m_callbacks)
            {
                callback(event);
            }
        }

    private:
        std::vector<std::function<void(const T &)>> m_callbacks;
    };
} // namespace he::event
