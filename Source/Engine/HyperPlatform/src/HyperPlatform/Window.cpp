/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "HyperPlatform/Window.hpp"

#include <GLFW/glfw3.h>

#include <utility>

namespace HyperPlatform
{
	Window::Window(const WindowInfo& t_window_info)
	{
		m_info.title = t_window_info.title;
		m_info.width = 1280;
		m_info.height = 720;
		m_info.event_manager = &t_window_info.event_manager;
		m_graphics_api = t_window_info.graphics_api;
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_native_window);
		glfwTerminate();
	}

	auto Window::initialize() -> HyperCore::InitializeResult
	{
		if (glfwInit() == GLFW_FALSE)
		{
			HyperCore::Logger::fatal("Failed to initialize GLFW");
			return HyperCore::ConstructError::Incomplete;
		}

		HyperCore::Logger::debug("GLFW was initialized");

		switch (m_graphics_api)
		{
		case GraphicsApi::OpenGL33:
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			break;
		case GraphicsApi::OpenGL46:
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			break;
		default:
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			break;
		}

		m_native_window = glfwCreateWindow(m_info.width, m_info.height, m_info.title.c_str(), nullptr, nullptr);
		if (m_native_window == nullptr)
		{
			glfwTerminate();

			HyperCore::Logger::fatal("Failed to create GLFW window");
			return HyperCore::ConstructError::Incomplete;
		}

		glfwSetWindowUserPointer(m_native_window, &m_info);

		glfwSetKeyCallback(
			m_native_window,
			[](GLFWwindow* window, int key, int, int action, int)
			{
				auto window_info = reinterpret_cast<Info*>(glfwGetWindowUserPointer(window));

				switch (action)
				{
				case GLFW_PRESS:
					window_info->event_manager->invoke<HyperCore::KeyPressedEvent>(key, 0);
					break;
				case GLFW_RELEASE:
					window_info->event_manager->invoke<HyperCore::KeyReleasedEvent>(key);
					break;
				case GLFW_REPEAT:
					window_info->event_manager->invoke<HyperCore::KeyPressedEvent>(key, 1);
					break;
				default:
					break;
				}
			});

		glfwSetCursorPosCallback(
			m_native_window,
			[](GLFWwindow* window, double x, double y)
			{
				auto window_info = reinterpret_cast<Info*>(glfwGetWindowUserPointer(window));
				window_info->event_manager->invoke<HyperCore::MouseMovedEvent>(static_cast<float>(x), static_cast<float>(y));
			});

		glfwSetScrollCallback(
			m_native_window,
			[](GLFWwindow* window, double x, double y)
			{
				auto window_info = reinterpret_cast<Info*>(glfwGetWindowUserPointer(window));
				window_info->event_manager->invoke<HyperCore::MouseScrolledEvent>(static_cast<float>(x), static_cast<float>(y));
			});

		glfwSetMouseButtonCallback(
			m_native_window,
			[](GLFWwindow* window, int button, int action, int)
			{
				auto window_info = reinterpret_cast<Info*>(glfwGetWindowUserPointer(window));

				switch (action)
				{
				case GLFW_PRESS:
					window_info->event_manager->invoke<HyperCore::MouseButtonPressedEvent>(button);
					break;
				case GLFW_RELEASE:
					window_info->event_manager->invoke<HyperCore::MouseButtonReleasedEvent>(button);
					break;
				default:
					break;
				}
			});

		glfwSetWindowCloseCallback(
			m_native_window,
			[](GLFWwindow* window)
			{
				auto window_info = reinterpret_cast<Info*>(glfwGetWindowUserPointer(window));
				window_info->event_manager->invoke<HyperCore::WindowCloseEvent>();
			});

		glfwSetWindowSizeCallback(
			m_native_window,
			[](GLFWwindow* window, int width, int height)
			{
				auto window_info = reinterpret_cast<Info*>(glfwGetWindowUserPointer(window));
				window_info->width = width;
				window_info->height = height;

				window_info->event_manager->invoke<HyperCore::WindowResizeEvent>(width, height);
			});

		glfwSetFramebufferSizeCallback(
			m_native_window,
			[](GLFWwindow* window, int width, int height)
			{
				auto window_info = reinterpret_cast<Info*>(glfwGetWindowUserPointer(window));
				window_info->width = width;
				window_info->height = height;

				window_info->event_manager->invoke<HyperCore::WindowFramebufferResizeEvent>(width, height);
			});

		glfwSetWindowFocusCallback(
			m_native_window,
			[](GLFWwindow* window, int focused)
			{
				auto window_info = reinterpret_cast<Info*>(glfwGetWindowUserPointer(window));

				if (focused == GLFW_TRUE)
				{
					window_info->event_manager->invoke<HyperCore::WindowFocusEvent>();
				}
				else if (focused == GLFW_FALSE)
				{
					window_info->event_manager->invoke<HyperCore::WindowLostFocusEvent>();
				}
			});

		glfwSetWindowPosCallback(
			m_native_window,
			[](GLFWwindow* window, int x, int y)
			{
				auto window_info = reinterpret_cast<Info*>(glfwGetWindowUserPointer(window));
				window_info->event_manager->invoke<HyperCore::WindowMovedEvent>(x, y);
			});

		HyperCore::Logger::info("Successfully created window");

		return {};
	}

	auto Window::poll_events() const -> void
	{
		glfwPollEvents();
	}

	auto Window::time() const -> float
	{
		return static_cast<float>(glfwGetTime());
	}

	auto Window::title() const -> std::string
	{
		return m_info.title;
	}

	auto Window::width() const -> int
	{
		return m_info.width;
	}

	auto Window::height() const -> int
	{
		return m_info.height;
	}

	auto Window::event_manager() -> HyperCore::EventManager*
	{
		return m_info.event_manager;
	}

	auto Window::native_window() const -> GLFWwindow*
	{
		return m_native_window;
	}
} // namespace HyperPlatform
