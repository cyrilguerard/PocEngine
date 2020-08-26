#include <optional>
#include <set>

#include "../constants.h"
#include "../debug.h"

#include "graphic.h"
#include "graphic-vulkan.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace poc::layers {

	constexpr const char* const debugLayer = "VK_LAYER_KHRONOS_validation";
	constexpr const char* const debugExtension = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	vk::DebugUtilsMessengerCreateInfoEXT createDebugMessengerCreateInfo(bool verbose = false);
	bool isDeviceSuitable(vk::PhysicalDevice device, vk::SurfaceKHR surface);
	std::optional<uint32_t> findSuitableQueueFamily(vk::PhysicalDevice device, vk::SurfaceKHR surface);
	bool isExtensionsSupportedBy(vk::PhysicalDevice device);
	bool isSurfaceSupportedBy(vk::PhysicalDevice device, vk::SurfaceKHR surface);

	GraphicVulkan::GraphicVulkan(const Window& window) : Graphic(Api::VULKAN) {
		initializeDispatcher();
		initializeInstance();
		if (isDebug) { initializeDebugMessenger(); }
		initializeSurface(window);
		initializePhysicalDevice();
		initializeDevice();
	}
	
	void GraphicVulkan::initializeDispatcher() {
		PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = loader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
	}

	void GraphicVulkan::initializeInstance() {

		const auto version = VK_MAKE_VERSION(
			poc::poc_engine_version_major,
			poc::poc_engine_version_minor,
			poc::poc_engine_version_minor);

		const auto appInfo = vk::ApplicationInfo()
			.setPApplicationName(poc::poc_engine_name)
			.setApplicationVersion(version)
			.setPEngineName(poc::poc_engine_name)
			.setEngineVersion(version)
			.setApiVersion(VK_HEADER_VERSION_COMPLETE);

		auto createInfo = vk::InstanceCreateInfo().setPApplicationInfo(&appInfo);

		std::vector<const char*> extensions{ };
		registerWindowVulkanExtensions(extensions);

		if (isDebug) {
			auto debugInfo = createDebugMessengerCreateInfo();
			createInfo.setPNext(&debugInfo);

			// enable layers only in debug
			createInfo.setEnabledLayerCount(1).setPpEnabledLayerNames(&debugLayer);

			// enable debug extension 
			extensions.push_back(debugExtension);
		}
		
		createInfo.setEnabledExtensionCount(static_cast<uint32_t>(extensions.size()));
		createInfo.setPpEnabledExtensionNames(extensions.data());

		instance = vk::createInstanceUnique(createInfo);
		VULKAN_HPP_DEFAULT_DISPATCHER.init(*instance);
		LOG("[POC::GraphicVulkan] Instance created")
	}

	void GraphicVulkan::initializeDebugMessenger() {
		assert(*instance && "instance not initialized");

		auto createInfo = createDebugMessengerCreateInfo();
		debugMessenger = instance->createDebugUtilsMessengerEXTUnique(createInfo);
		LOG("[POC::GraphicVulkan] Debug messenger created")
	}

	VkBool32 VKAPI_PTR debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT			messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT					messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {
		auto ms = static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity);
		auto mt = static_cast<vk::DebugUtilsMessageTypeFlagBitsEXT>(messageTypes);
		LOG("[POC::Graphic::Vulkan][" << vk::to_string(mt) << "][" << vk::to_string(ms) << "] " << pCallbackData->pMessage)
		return VK_FALSE;
	}

	vk::DebugUtilsMessengerCreateInfoEXT createDebugMessengerCreateInfo(bool verbose) {

		vk::DebugUtilsMessageSeverityFlagsEXT severity =
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;

		if (verbose) {
			severity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;
			severity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose;
		}

		return vk::DebugUtilsMessengerCreateInfoEXT()
			.setMessageSeverity(severity)
			.setMessageType(
				vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
				vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
				vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
			.setPfnUserCallback(debugCallback);

	}

	void GraphicVulkan::initializeSurface(const Window& window) {
		assert(*instance && "instance not initialized");
		VkSurfaceKHR s{};
		VkInstance i = *instance;
		window.exposeToGraphicApi(i, &s);
		
		auto deleter = vk::UniqueHandleTraits<vk::SurfaceKHR, vk::DispatchLoaderDynamic>::deleter(*instance);
		surface = vk::UniqueSurfaceKHR(s, deleter);
		LOG("[POC::GraphicVulkan] Surface created")
	}

	void GraphicVulkan::initializePhysicalDevice() {
		assert(*instance && "instance not initialized");
		assert(*surface && "surface not initialized");

		const auto devices = instance->enumeratePhysicalDevices();
		if (devices.empty()) {
			throw std::runtime_error("No GPU available for Vulkan");
		}

		for (const auto device : devices) {
			if (isDeviceSuitable(device, *surface)) {
				physicalDevice = device;
				LOG("[POC::Graphic::Vulkan] Use device: " << device.getProperties().deviceName)
				break;
			}
		}

		if (!physicalDevice) {
			throw std::runtime_error("No suitable GPU found");
		}

	}

	bool isDeviceSuitable(const vk::PhysicalDevice device, const vk::SurfaceKHR surface) {
		return findSuitableQueueFamily(device, surface).has_value() &&
			isExtensionsSupportedBy(device) &&
			isSurfaceSupportedBy(device, surface);
	}


	std::optional<uint32_t> findSuitableQueueFamily(const vk::PhysicalDevice device, const vk::SurfaceKHR surface) {
		uint32_t i = 0;
		for (const auto& queueFamily : device.getQueueFamilyProperties()) {
			if ((queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) && device.getSurfaceSupportKHR(i, surface)) {
				return std::make_optional<uint32_t>(i);
			}
			i++;
		}
		return std::make_optional<uint32_t>();
	}
	
	bool isExtensionsSupportedBy(const vk::PhysicalDevice device) {
		std::vector<vk::ExtensionProperties> extensions = device.enumerateDeviceExtensionProperties();
		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
		for (const auto& extension : extensions) {
			requiredExtensions.erase(extension.extensionName.data());
		}
		return requiredExtensions.empty();
	}

	bool isSurfaceSupportedBy(const vk::PhysicalDevice device, const vk::SurfaceKHR surface) {
		auto formats = device.getSurfaceFormatsKHR(surface);
		if (formats.empty()) { return false; }

		auto modes = device.getSurfacePresentModesKHR(surface);
		if (modes.empty()) { return false; }

		return true;
	}

	void GraphicVulkan::initializeDevice() {
		assert(physicalDevice && "physicalDevice not initialized");
		assert(*surface && "surface not initialized");

		std::optional<uint32_t> queueIndex = findSuitableQueueFamily(physicalDevice, *surface);
		assert(queueIndex && "no suitable queue found");

		float queuePriority = 1.0f;
		auto queueInfo = vk::DeviceQueueCreateInfo()
			.setQueueFamilyIndex(queueIndex.value())
			.setPQueuePriorities(&queuePriority)
			.setQueueCount(1);

		auto createInfo = vk::DeviceCreateInfo()
			.setQueueCreateInfoCount(1)
			.setPQueueCreateInfos(&queueInfo)
			.setEnabledExtensionCount(static_cast<uint32_t>(deviceExtensions.size()))
			.setPpEnabledExtensionNames(deviceExtensions.data());

		device = physicalDevice.createDeviceUnique(createInfo);
		graphicQueue = device->getQueue(queueIndex.value(), 0);
		presentQueue = device->getQueue(queueIndex.value(), 0);
		LOG("[POC::GraphicVulkan] Device created")
	}

}