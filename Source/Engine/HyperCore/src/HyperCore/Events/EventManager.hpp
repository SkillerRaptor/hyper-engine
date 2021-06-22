/*
 * Copyright (c) 2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <HyperCore/Logger.hpp>
#include <HyperCore/Events/IEvent.hpp>
#include <HyperCore/Events/EventWrapper.hpp>
#include <functional>
#include <type_traits>
#include <unordered_map>

namespace HyperCore
{
	class CEventFamilyGenerator
	{
	public:
		using EventIdType = uint16_t;

	public:
		template <typename>
		static EventIdType type()
		{
			static const EventIdType identifier =
				CEventFamilyGenerator::identifier();
			return identifier;
		}

	private:
		static EventIdType identifier();
	};

	class CEventManager
	{
	public:
		template <typename T>
		void invoke(const T& event)
		{
			static_assert(
				std::is_base_of_v<IEvent, T>,
				"Template argument T is not base of IEvent");

			const CEventFamilyGenerator::EventIdType event_id =
				CEventFamilyGenerator::type<T>();
			if (m_event_wrappers.find(event_id) == m_event_wrappers.end())
			{
				m_event_wrappers[event_id] = new CEventWrapper<T>();
			}

			IEventWrapperBase* event_wrapper_base = m_event_wrappers[event_id];
			CEventWrapper<T>* event_wrapper =
				static_cast<CEventWrapper<T>*>(event_wrapper_base);
			event_wrapper->invoke(event);
		}

		template <typename T, typename... Args>
		void invoke(Args&&... args)
		{
			static_assert(
				std::is_base_of_v<IEvent, T>,
				"Template argument T is not base of IEvent");

			const CEventFamilyGenerator::EventIdType event_id =
				CEventFamilyGenerator::type<T>();
			if (m_event_wrappers.find(event_id) == m_event_wrappers.end())
			{
				m_event_wrappers[event_id] = new CEventWrapper<T>();
			}

			IEventWrapperBase* event_wrapper_base = m_event_wrappers[event_id];
			CEventWrapper<T>* event_wrapper =
				static_cast<CEventWrapper<T>*>(event_wrapper_base);
			event_wrapper->invoke(T(std::forward<Args>(args)...));
		}

		template <class T>
		void register_listener(
			const std::string& name,
			const std::function<void(const T&)>& event_listener)
		{
			static_assert(
				std::is_base_of_v<IEvent, T>,
				"Template argument T is not base of IEvent");

			const CEventFamilyGenerator::EventIdType event_id =
				CEventFamilyGenerator::type<T>();
			if (m_event_wrappers.find(event_id) == m_event_wrappers.end())
			{
				m_event_wrappers[event_id] = new CEventWrapper<T>();
			}

			IEventWrapperBase* event_wrapper_base = m_event_wrappers[event_id];
			CEventWrapper<T>* event_wrapper =
				static_cast<CEventWrapper<T>*>(event_wrapper_base);
			event_wrapper->register_listener(name, event_listener);
		}

		template <class T>
		void unregister_listener(const std::string& name)
		{
			static_assert(
				std::is_base_of_v<IEvent, T>,
				"Template argument T is not base of IEvent");

			const CEventFamilyGenerator::EventIdType event_id =
				CEventFamilyGenerator::type<T>();
			if (m_event_wrappers.find(event_id) == m_event_wrappers.end())
			{
				CLogger::error(
					"Failed to unregister event listener: '{}' not registered!",
					name);
				return;
			}

			IEventWrapperBase* event_wrapper_base = m_event_wrappers[event_id];
			CEventWrapper<T>* event_wrapper =
				static_cast<CEventWrapper<T>*>(event_wrapper_base);
			event_wrapper->unregister_listener(name);
		}

	private:
		std::unordered_map<
			CEventFamilyGenerator::EventIdType,
			IEventWrapperBase*>
			m_event_wrappers{};
	};
} // namespace HyperCore
