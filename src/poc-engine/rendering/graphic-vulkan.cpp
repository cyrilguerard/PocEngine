#include "../constants.h"
#include "../debug.h"

#include "graphic.h"
#include "graphic-vulkan.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace poc::layers {

	constexpr const char* const debugLayer = "VK_LAYER_KHRONOS_validation";
	constexpr const char* const debugExtension = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

	vk::DebugUtilsMessengerCreateInfoEXT createDebugMessengerCreateInfo(bool verbose = false);

	GraphicVulkan::GraphicVulkan() : Graphic(Api::VULKAN) {
		initializeDispatcher();
		initializeInstance();
		if (isDebug) { initializeDebugMessenger(); }
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

}