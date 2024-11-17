/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include "hyper_event/event_handler.hpp"
#include "hyper_event/event_id_generator.hpp"

namespace he::event
{
    class EventBus
    {
    public:
        template <typename T, typename... Args>
        void dispatch(Args &&...args)
        {
            const size_t event_id = EventIdGenerator::type<T>();
            if (!m_handlers.contains(event_id))
            {
                return;
            }

            const std::unique_ptr<EventHandler> &handler = m_handlers[event_id];
            auto *event_handler = static_cast<EventHandlerImpl<T> *>(handler.get());
            event_handler->dispatch(T(std::forward<Args>(args)...));
        }

        template <typename T>
        void subscribe(const std::function<void(const T &)> &callback)
        {
            const size_t event_id = EventIdGenerator::type<T>();
            if (!m_handlers.contains(event_id))
            {
                m_handlers.insert({ event_id, std::make_unique<EventHandlerImpl<T>>() });
            }

            const std::unique_ptr<EventHandler> &handler = m_handlers[event_id];
            auto *event_handler = static_cast<EventHandlerImpl<T> *>(handler.get());
            event_handler->subscribe(callback);
        }

    private:
        std::unordered_map<size_t, std::unique_ptr<EventHandler>> m_handlers;
    };
} // namespace he::event
