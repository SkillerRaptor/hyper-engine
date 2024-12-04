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

            const std::unique_ptr<IEventHandler> &handler = m_handlers[event_id];
            auto *event_handler = static_cast<EventHandler<T> *>(handler.get());
            event_handler->dispatch(T(std::forward<Args>(args)...));
        }

        // TODO: To prevent functions being called which are no longer available, add an id system to unsubscribe event handlers
        template <typename T>
        void subscribe(const std::function<void(const T &)> &callback)
        {
            const size_t event_id = EventIdGenerator::type<T>();
            if (!m_handlers.contains(event_id))
            {
                m_handlers.insert({event_id, std::make_unique<EventHandler<T>>()});
            }

            const std::unique_ptr<IEventHandler> &handler = m_handlers[event_id];
            auto *event_handler = static_cast<EventHandler<T> *>(handler.get());
            event_handler->subscribe(callback);
        }

    private:
        std::unordered_map<size_t, std::unique_ptr<IEventHandler>> m_handlers;
    };
} // namespace hyper_engine
