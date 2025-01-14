/*
 * Copyright (c) 2024-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <functional>
#include <unordered_map>

#include <hyper_core/own_ptr.hpp>

#include "hyper_event/event_handler.hpp"
#include "hyper_event/event_id_generator.hpp"

namespace hyper_engine
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

            const OwnPtr<EventHandler> &handler = m_handlers.at(event_id);
            EventHandlerImpl<T> *event_handler = static_cast<EventHandlerImpl<T> *>(handler.get());
            event_handler->dispatch(T(std::forward<Args>(args)...));
        }

        // FIXME: To prevent functions being called which are no longer available, add an id system to unsubscribe event handlers
        template <typename T>
        void subscribe(const std::function<void(const T &)> &callback)
        {
            const size_t event_id = EventIdGenerator::type<T>();
            if (!m_handlers.contains(event_id))
            {
                m_handlers.insert({event_id, make_own<EventHandlerImpl<T>>()});
            }

            const OwnPtr<EventHandler> &handler = m_handlers.at(event_id);
            EventHandlerImpl<T> *event_handler = static_cast<EventHandlerImpl<T> *>(handler.get());
            event_handler->subscribe(callback);
        }

        static EventBus *&get()
        {
            static EventBus *event_bus = nullptr;
            return event_bus;
        }

    private:
        std::unordered_map<size_t, OwnPtr<EventHandler>> m_handlers;
    };
} // namespace hyper_engine
