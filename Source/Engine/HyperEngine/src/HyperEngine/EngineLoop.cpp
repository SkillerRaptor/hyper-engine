/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "HyperEngine/EngineLoop.hpp"

#include "HyperEngine/IApplication.hpp"

namespace HyperEngine
{
	EngineLoop::EngineLoop(IApplication& application)
		: m_application(application)
		, m_window(m_application.title(), m_application.graphics_api())
	{
	}

	auto EngineLoop::initialize() -> HyperCore::InitializeResult
	{
		initialize_event_callbacks();

		m_event_manager.register_listener<HyperGame::WindowCloseEvent>(
			[this](const HyperGame::WindowCloseEvent& window_close_event)
			{
				HYPERENGINE_VARIABLE_NOT_USED(window_close_event);

				m_running = false;
			});

		auto window_result = m_window.initialize();
		if (window_result.is_error())
		{
			HyperCore::Logger::fatal("Failed to create window");
			return window_result.error();
		}

		m_running = true;

		return {};
	}

	auto EngineLoop::initialize_event_callbacks() -> void
	{
		m_window.set_key_pressed_callback(
			[this](int32_t key, bool repeat)
			{
				m_event_manager.invoke<HyperGame::KeyPressedEvent>(key, repeat);
			});

		m_window.set_key_released_callback(
			[this](int32_t key)
			{
				m_event_manager.invoke<HyperGame::KeyReleasedEvent>(key);
			});

		m_window.set_mouse_moved_callback(
			[this](float x, float y)
			{
				m_event_manager.invoke<HyperGame::MouseMovedEvent>(x, y);
			});

		m_window.set_mouse_scrolled_callback(
			[this](float x, float y)
			{
				m_event_manager.invoke<HyperGame::MouseScrolledEvent>(x, y);
			});

		m_window.set_mouse_button_pressed_callback(
			[this](int32_t button)
			{
				m_event_manager.invoke<HyperGame::MouseButtonPressedEvent>(static_cast<HyperGame::MouseButton>(button));
			});

		m_window.set_mouse_button_released_callback(
			[this](int32_t button)
			{
				m_event_manager.invoke<HyperGame::MouseButtonReleasedEvent>(static_cast<HyperGame::MouseButton>(button));
			});

		m_window.set_window_close_callback(
			[this]()
			{
				m_event_manager.invoke<HyperGame::WindowCloseEvent>();
			});

		m_window.set_window_resize_callback(
			[this](int32_t width, int32_t height)
			{
				m_event_manager.invoke<HyperGame::WindowResizeEvent>(width, height);
			});

		m_window.set_window_framebuffer_resize_callback(
			[this](int32_t width, int32_t height)
			{
				m_event_manager.invoke<HyperGame::WindowFramebufferResizeEvent>(width, height);
			});

		m_window.set_window_focus_callback(
			[this]()
			{
				m_event_manager.invoke<HyperGame::WindowLostFocusEvent>();
			});

		m_window.set_window_lost_focus_callback(
			[this]()
			{
				m_event_manager.invoke<HyperGame::WindowLostFocusEvent>();
			});

		m_window.set_window_moved_callback(
			[this](int32_t x, int32_t y)
			{
				m_event_manager.invoke<HyperGame::WindowMovedEvent>(x, y);
			});
	}

	auto EngineLoop::terminate() -> void
	{
	}

	auto EngineLoop::run() -> void
	{
		auto last_time = 0.0F;
		while (m_running)
		{
			auto current_time = m_window.time();
			auto delta_time = last_time - current_time;
			last_time = current_time;

			(void) delta_time;

			m_event_manager.process_events();
			m_window.poll_events();
		}
	}
} // namespace HyperEngine
