/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "HyperEngine/Rendering/RenderContext.hpp"

#include "HyperEngine/Platform/Window.hpp"
#include "HyperEngine/Rendering/Device.hpp"
#include "HyperEngine/Rendering/SwapChain.hpp"
#include "HyperEngine/Rendering/Utils.hpp"

#include <cstring>
#include <GLFW/glfw3.h>
#include <volk.h>

namespace HyperEngine
{
	RenderContext::RenderContext(
		bool request_validation_layers,
		const Window &window,
		Error &error)
		: m_window(&window)
	{
		if (volkInitialize() != VK_SUCCESS)
		{
			error = Error("failed to initialize volk");
			return;
		}

		m_validation_layers_enabled =
			request_validation_layers && validation_layers_supported();

		const auto instance = create_instance();
		if (instance.is_error())
		{
			error = instance.error();
			return;
		}

		if (m_validation_layers_enabled)
		{
			const auto debug_messenger = create_debug_messenger();
			if (debug_messenger.is_error())
			{
				error = debug_messenger.error();
				return;
			}
		}

		const auto surface = m_window->create_surface(m_instance);
		if (surface.is_error())
		{
			error = surface.error();
			return;
		}

		m_surface = surface.value();

		auto device = Device::create(m_instance, m_surface);
		if (device.is_error())
		{
			error = device.error();
			return;
		}

		m_device = device.value();

		auto swap_chain = SwapChain::create(m_surface, *m_device, *m_window);
		if (swap_chain.is_error())
		{
			error = swap_chain.error();
			return;
		}

		m_swap_chain = swap_chain.value();
	}

	RenderContext::~RenderContext()
	{
		if (m_swap_chain != nullptr)
		{
			delete m_swap_chain;
		}

		if (m_device != nullptr)
		{
			delete m_device;
		}

		if (m_surface != nullptr)
		{
			vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		}

		if (m_debug_messenger != nullptr)
		{
			vkDestroyDebugUtilsMessengerEXT(m_instance, m_debug_messenger, nullptr);
		}

		if (m_instance != nullptr)
		{
			vkDestroyInstance(m_instance, nullptr);
		}
	}

	RenderContext::RenderContext(RenderContext &&other) noexcept
		: m_window(std::exchange(other.m_window, nullptr))
		, m_instance(std::exchange(other.m_instance, nullptr))
		, m_debug_messenger(std::exchange(other.m_debug_messenger, nullptr))
		, m_surface(std::exchange(other.m_surface, nullptr))
		, m_device(std::exchange(other.m_device, nullptr))
		, m_swap_chain(std::exchange(other.m_swap_chain, nullptr))
	{
	}

	RenderContext &RenderContext::operator=(RenderContext &&other) noexcept
	{
		m_window = std::exchange(other.m_window, nullptr);
		m_instance = std::exchange(other.m_instance, nullptr);
		m_debug_messenger = std::exchange(other.m_debug_messenger, nullptr);
		m_surface = std::exchange(other.m_surface, nullptr);
		m_device = std::exchange(other.m_device, nullptr);
		m_swap_chain = std::exchange(other.m_swap_chain, nullptr);
		return *this;
	}

	Expected<void> RenderContext::create_instance()
	{
		const VkApplicationInfo application_info = {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext = nullptr,
			.pApplicationName = "HyperEditor",
			.applicationVersion = Utils::make_api_version(0, 1, 0, 0),
			.pEngineName = "HyperEngine",
			.engineVersion = Utils::make_api_version(0, 1, 0, 0),
			.apiVersion = Utils::make_api_version(0, 1, 2, 0),
		};

		const VkDebugUtilsMessageSeverityFlagsEXT message_severity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		const VkDebugUtilsMessageTypeFlagsEXT message_type =
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		const VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info = {
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			.pNext = nullptr,
			.flags = 0,
			.messageSeverity = message_severity,
			.messageType = message_type,
			.pfnUserCallback = Utils::debug_callback,
			.pUserData = nullptr,
		};

		const size_t layer_count =
			m_validation_layers_enabled ? s_validation_layers.size() : 0;
		const char *const *layers =
			m_validation_layers_enabled ? s_validation_layers.data() : nullptr;
		const std::vector<const char *> extensions = get_required_extensions();
		const VkInstanceCreateInfo instance_create_info = {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = m_validation_layers_enabled ? &debug_messenger_info : nullptr,
			.flags = 0,
			.pApplicationInfo = &application_info,
			.enabledLayerCount = static_cast<uint32_t>(layer_count),
			.ppEnabledLayerNames = layers,
			.enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
			.ppEnabledExtensionNames = extensions.data(),
		};

		const auto result =
			vkCreateInstance(&instance_create_info, nullptr, &m_instance);
		if (result != VK_SUCCESS)
		{
			return Error("failed to create instance");
		}

		volkLoadInstance(m_instance);

		return {};
	}

	Expected<void> RenderContext::create_debug_messenger()
	{
		const VkDebugUtilsMessageSeverityFlagsEXT message_severity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		const VkDebugUtilsMessageTypeFlagsEXT message_type =
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		const VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info = {
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			.pNext = nullptr,
			.flags = 0,
			.messageSeverity = message_severity,
			.messageType = message_type,
			.pfnUserCallback = Utils::debug_callback,
			.pUserData = nullptr,
		};

		const auto result = vkCreateDebugUtilsMessengerEXT(
			m_instance, &debug_messenger_info, nullptr, &m_debug_messenger);
		if (result != VK_SUCCESS)
		{
			return Error("failed to create debug messenger");
		}

		return {};
	}

	bool RenderContext::validation_layers_supported() const noexcept
	{
		uint32_t layer_count = 0;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
		if (layer_count == 0)
		{
			return false;
		}

		std::vector<VkLayerProperties> layers(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, layers.data());

		for (const char *validation_layer : s_validation_layers)
		{
			for (const VkLayerProperties &properties : layers)
			{
				if (strcmp(validation_layer, properties.layerName) == 0)
				{
					continue;
				}

				return true;
			}
		}

		return false;
	}

	std::vector<const char *> RenderContext::get_required_extensions() const
	{
		uint32_t required_extension_count = 0;
		const char **required_extensions =
			glfwGetRequiredInstanceExtensions(&required_extension_count);
		if (required_extension_count == 0)
		{
			return {};
		}

		std::vector<const char *> extensions(
			required_extensions, required_extensions + required_extension_count);
		if (m_validation_layers_enabled)
		{
			extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	Expected<RenderContext *> RenderContext::create(
		bool request_validation_layers,
		const Window &window)
	{
		Error error = Error::success();
		auto *render_context =
			new RenderContext(request_validation_layers, window, error);
		if (error.is_error())
		{
			delete render_context;
			return error;
		}

		return render_context;
	}
} // namespace HyperEngine
