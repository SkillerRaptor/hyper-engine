/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "HyperVulkan/GraphicsContext.hpp"

#include <GLFW/glfw3.h>

namespace HyperRendering::HyperVulkan
{
	GraphicsContext::GraphicsContext(HyperGame::EventManager& t_event_manager, HyperPlatform::Window& t_window)
		: IGraphicsContext(t_event_manager, t_window)
		, m_device(m_instance, m_surface)
		, m_swap_chain(m_window, m_device, m_surface)
		, m_graphics_pipeline(m_device.device(), m_swap_chain)
	{
	}

	GraphicsContext::~GraphicsContext()
	{
		auto destroy_surface = [this]() -> bool
		{
			if (m_surface == VK_NULL_HANDLE)
			{
				return false;
			}

			vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
			HyperCore::Logger::debug("Vulkan surface was destroyed");

			return true;
		};

		auto destroy_debug_messenger = [this]() -> bool
		{
			if (m_validation_layers_enabled)
			{
				if (m_debug_messenger == VK_NULL_HANDLE)
				{
					return false;
				}

				vkDestroyDebugUtilsMessengerEXT(m_instance, m_debug_messenger, nullptr);
				HyperCore::Logger::debug("Vulkan debug messenger was destroyed");
			}

			return true;
		};

		auto destroy_instance = [this]() -> bool
		{
			if (m_instance == VK_NULL_HANDLE)
			{
				return false;
			}

			vkDestroyInstance(m_instance, nullptr);
			HyperCore::Logger::debug("Vulkan instance was destroyed");

			return true;
		};
		
		if (!m_graphics_pipeline.destroy())
		{
			return;
		}

		if (!m_swap_chain.destroy())
		{
			return;
		}

		if (!m_device.destroy())
		{
			return;
		}

		if (!destroy_surface())
		{
			return;
		}

		if (!destroy_debug_messenger())
		{
			return;
		}

		if (!destroy_instance())
		{
			return;
		}

		HyperCore::Logger::info("Successfully destroyed Vulkan context");
	}

	auto GraphicsContext::initialize() -> bool
	{
		if (volkInitialize() != VK_SUCCESS)
		{
			HyperCore::Logger::fatal("GraphicsContext::initialize(): Failed to initialize volk");
			return false;
		}

		HyperCore::Logger::debug("Volk was initialized");

		if (m_validation_layers_enabled && !validation_layers_supported())
		{
			HyperCore::Logger::warning("Vulkan validation layer were requested, but are not supported!");
			m_validation_layers_enabled = false;
		}

		if (!create_instance())
		{
			HyperCore::Logger::fatal("GraphicsContext::initialize(): Failed to create vulkan instance");
			return false;
		}

		if (m_validation_layers_enabled)
		{
			if (!create_debug_messenger())
			{
				HyperCore::Logger::fatal("GraphicsContext::initialize(): Failed to create vulkan debug messenger");
				return false;
			}
		}

		if (!create_surface())
		{
			HyperCore::Logger::fatal("GraphicsContext::initialize(): Failed to create vulkan surface");
			return false;
		}

		if (!m_device.initialize())
		{
			HyperCore::Logger::fatal("GraphicsContext::initialize(): Failed to create device");
			return false;
		}

		if (!m_swap_chain.initialize())
		{
			HyperCore::Logger::fatal("GraphicsContext::initialize(): Failed to create swap chain");
			return false;
		}
		
		if (!m_graphics_pipeline.initialize())
		{
			HyperCore::Logger::fatal("GraphicsContext::initialize(): Failed to create graphics pipeline");
			return false;
		}

		HyperCore::Logger::info("Successfully created Vulkan context");

		return true;
	}

	auto GraphicsContext::update() -> void
	{
	}

	auto GraphicsContext::create_instance() -> bool
	{
		VkApplicationInfo application_info{};
		application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		application_info.pApplicationName = m_window.title().c_str();
		application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		application_info.pEngineName = "HyperEngine";
		application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		application_info.apiVersion = VK_API_VERSION_1_2;

		auto required_extensions = get_required_extensions();

		VkInstanceCreateInfo instance_create_info{};
		instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_create_info.pApplicationInfo = &application_info;
		instance_create_info.ppEnabledExtensionNames = required_extensions.data();
		instance_create_info.enabledExtensionCount = static_cast<uint32_t>(required_extensions.size());
		instance_create_info.ppEnabledLayerNames = nullptr;
		instance_create_info.enabledLayerCount = 0;
		instance_create_info.pNext = nullptr;

		VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info{};
		if (m_validation_layers_enabled)
		{
			instance_create_info.ppEnabledLayerNames = s_validation_layers.data();
			instance_create_info.enabledLayerCount = static_cast<uint32_t>(s_validation_layers.size());

			debug_messenger_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debug_messenger_create_info.messageSeverity =
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debug_messenger_create_info.messageType =
				VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debug_messenger_create_info.pfnUserCallback = debug_callback;

			instance_create_info.pNext = &debug_messenger_create_info;
		}

		if (vkCreateInstance(&instance_create_info, nullptr, &m_instance) != VK_SUCCESS)
		{
			HyperCore::Logger::fatal("GraphicsContext::create_instance(): Failed to create Vulkan instance");
			return false;
		}

		volkLoadInstance(m_instance);

		HyperCore::Logger::debug("Vulkan instance was created");

		return true;
	}

	auto GraphicsContext::create_debug_messenger() -> bool
	{
		VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info{};
		debug_messenger_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debug_messenger_create_info.messageSeverity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debug_messenger_create_info.messageType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debug_messenger_create_info.pfnUserCallback = debug_callback;

		if (vkCreateDebugUtilsMessengerEXT(m_instance, &debug_messenger_create_info, nullptr, &m_debug_messenger) != VK_SUCCESS)
		{
			HyperCore::Logger::fatal("GraphicsContext::create_debug_messenger(): Failed to create Vulkan debug messenger");
			return false;
		}

		HyperCore::Logger::debug("Vulkan debug messenger was created");

		return true;
	}

	auto GraphicsContext::create_surface() -> bool
	{
		if (glfwCreateWindowSurface(m_instance, m_window.native_window(), nullptr, &m_surface) != VK_SUCCESS)
		{
			HyperCore::Logger::fatal("GraphicsContext::create_surface(): Failed to create Vulkan surface");
			return false;
		}

		HyperCore::Logger::debug("Vulkan surface was created");

		return true;
	}

	auto GraphicsContext::validation_layers_supported() const -> bool
	{
		uint32_t available_layer_count = 0;
		vkEnumerateInstanceLayerProperties(&available_layer_count, nullptr);

		std::vector<VkLayerProperties> available_layer_properties(available_layer_count);
		vkEnumerateInstanceLayerProperties(&available_layer_count, available_layer_properties.data());

		for (const char* validation_layer : s_validation_layers)
		{
			bool layer_found = false;

			for (const auto& layer_properties : available_layer_properties)
			{
				if (strcmp(validation_layer, layer_properties.layerName) != 0)
				{
					continue;
				}

				layer_found = true;
				break;
			}

			if (!layer_found)
			{
				return false;
			}
		}

		return true;
	}

	auto GraphicsContext::get_required_extensions() const -> std::vector<const char*>
	{
		uint32_t required_instance_extension_count = 0;
		const char** required_instance_extensions;
		required_instance_extensions = glfwGetRequiredInstanceExtensions(&required_instance_extension_count);

		std::vector<const char*> required_extensions(required_instance_extensions, required_instance_extensions + required_instance_extension_count);

		if (m_validation_layers_enabled)
		{
			required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return required_extensions;
	}

	auto GraphicsContext::debug_callback(
		VkDebugUtilsMessageSeverityFlagBitsEXT severity_flags,
		VkDebugUtilsMessageTypeFlagsEXT type_flags,
		const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
		void* user_data) -> VkBool32
	{
		HYPERENGINE_VARIABLE_NOT_USED(type_flags);
		HYPERENGINE_VARIABLE_NOT_USED(user_data);

		if (severity_flags >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			HyperCore::Logger::fatal("GraphicsContext::debug_callback(): {}", callback_data->pMessage);
		}
		else if (severity_flags >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			HyperCore::Logger::warning("GraphicsContext::debug_callback(): {}", callback_data->pMessage);
		}

		return VK_FALSE;
	}
} // namespace HyperRendering::HyperVulkan
