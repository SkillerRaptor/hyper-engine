/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "hyper_platform/window.hpp"

#include <GLFW/glfw3.h>

#include <hyper_core/assertion.hpp>
#include <hyper_core/logger.hpp>
#include <hyper_event/event_bus.hpp>

#include "hyper_platform/key_events.hpp"
#include "hyper_platform/mouse_events.hpp"
#include "hyper_platform/window_events.hpp"

namespace hyper_platform
{
    Window::Window(const WindowDescriptor &descriptor)
        : m_window(nullptr)
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_window =
            glfwCreateWindow(static_cast<int>(descriptor.width), static_cast<int>(descriptor.height), descriptor.title.data(), nullptr, nullptr);
        HE_ASSERT(m_window);

        glfwSetWindowUserPointer(m_window, &descriptor.event_bus);

        glfwSetWindowSizeCallback(
            m_window,
            [](GLFWwindow *window, int width, int height)
            {
                hyper_event::EventBus &event_bus = *static_cast<hyper_event::EventBus *>(glfwGetWindowUserPointer(window));
                event_bus.dispatch(hyper_platform::WindowResizeEvent(static_cast<uint32_t>(width), static_cast<uint32_t>(height)));
            });

        glfwSetFramebufferSizeCallback(
            m_window,
            [](GLFWwindow *window, int width, int height)
            {
                hyper_event::EventBus &event_bus = *static_cast<hyper_event::EventBus *>(glfwGetWindowUserPointer(window));
                event_bus.dispatch(hyper_platform::WindowFramebufferResizeEvent(static_cast<uint32_t>(width), static_cast<uint32_t>(height)));
            });

        glfwSetWindowCloseCallback(
            m_window,
            [](GLFWwindow *window)
            {
                hyper_event::EventBus &event_bus = *static_cast<hyper_event::EventBus *>(glfwGetWindowUserPointer(window));
                event_bus.dispatch(hyper_platform::WindowCloseEvent());
            });

        glfwSetKeyCallback(
            m_window,
            [](GLFWwindow *window, int key, int, int action, int)
            {
                hyper_event::EventBus &event_bus = *static_cast<hyper_event::EventBus *>(glfwGetWindowUserPointer(window));

                switch (action)
                {
                case GLFW_PRESS:
                    event_bus.dispatch(hyper_platform::KeyPressedEvent(static_cast<KeyCode>(key)));
                    break;
                case GLFW_RELEASE:
                    event_bus.dispatch(hyper_platform::KeyReleasedEvent(static_cast<KeyCode>(key)));
                    break;
                default:
                    break;
                }
            });

        glfwSetMouseButtonCallback(
            m_window,
            [](GLFWwindow *window, int button, int action, int)
            {
                hyper_event::EventBus &event_bus = *static_cast<hyper_event::EventBus *>(glfwGetWindowUserPointer(window));

                switch (action)
                {
                case GLFW_PRESS:
                    event_bus.dispatch(hyper_platform::MouseButtonPressedEvent(static_cast<MouseCode>(button)));
                    break;
                case GLFW_RELEASE:
                    event_bus.dispatch(hyper_platform::MouseButtonReleasedEvent(static_cast<MouseCode>(button)));
                    break;
                default:
                    break;
                }
            });

        glfwSetScrollCallback(
            m_window,
            [](GLFWwindow *window, double delta_x, double delta_y)
            {
                hyper_event::EventBus &event_bus = *static_cast<hyper_event::EventBus *>(glfwGetWindowUserPointer(window));
                event_bus.dispatch(hyper_platform::MouseScrolledEvent(static_cast<float>(delta_x), static_cast<float>(delta_y)));
            });

        glfwSetCursorPosCallback(
            m_window,
            [](GLFWwindow *window, double x, double y)
            {
                hyper_event::EventBus &event_bus = *static_cast<hyper_event::EventBus *>(glfwGetWindowUserPointer(window));
                event_bus.dispatch(hyper_platform::MouseMovedEvent(static_cast<float>(x), static_cast<float>(y)));
            });

        HE_DEBUG("Window created | title=\"{}\" width={} height={}", descriptor.title, descriptor.width, descriptor.height);
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::poll_events()
    {
        glfwPollEvents();
    }
} // namespace hyper_platform
