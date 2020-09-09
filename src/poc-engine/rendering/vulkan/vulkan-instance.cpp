#include "vulkan-instance.hpp"

#include "../../constants.hpp"
#include "../../core/logger.hpp"

using namespace poc;

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanInstance" };

	static vk::DynamicLoader createLoader() {
		vk::DynamicLoader loader = vk::DynamicLoader();
		PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = loader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
		return loader;
	}

	VkBool32 VKAPI_PTR debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void*) {

		const auto mt = static_cast<vk::DebugUtilsMessageTypeFlagBitsEXT>(messageTypes);

		const std::string tag = "Vulkan/" + vk::to_string(mt);
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			Logger::debug(tag, pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			Logger::warn(tag, pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			Logger::error(tag, pCallbackData->pMessage);
			break;
		default:
			break;
		}

		return VK_FALSE;
	}

	static vk::DebugUtilsMessengerCreateInfoEXT createDebugMessengerCreateInfo() {
		return vk::DebugUtilsMessengerCreateInfoEXT()
			.setMessageSeverity(
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
			)
			.setMessageType(
				vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
				vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
				vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
			)
			.setPfnUserCallback(debugCallback);
	}

	static vk::UniqueInstance createInstance() {

		const auto version = VK_MAKE_VERSION(
			poc::engine_version_major,
			poc::engine_version_minor,
			poc::engine_version_minor);

		const auto appInfo = vk::ApplicationInfo()
			.setPApplicationName(poc::engine_name)
			.setApplicationVersion(version)
			.setPEngineName(poc::engine_name)
			.setEngineVersion(version)
			.setApiVersion(VK_HEADER_VERSION_COMPLETE);

		auto createInfo = vk::InstanceCreateInfo().setPApplicationInfo(&appInfo);

		std::vector<const char*> extensions{};
		registerWindowVulkanExtensions(extensions);

		if (isDebug) {
			auto debugInfo = createDebugMessengerCreateInfo();
			createInfo.setPNext(&debugInfo);

			// enable layers only in debug
			auto debugLayer = "VK_LAYER_KHRONOS_validation";
			createInfo.setEnabledLayerCount(1).setPpEnabledLayerNames(&debugLayer);

			// enable debug extension
			auto debugExtension = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
			extensions.push_back(debugExtension);
		}

		createInfo.setEnabledExtensionCount(static_cast<uint32_t>(extensions.size()));
		createInfo.setPpEnabledExtensionNames(extensions.data());

		auto instance = vk::createInstanceUnique(createInfo);
		VULKAN_HPP_DEFAULT_DISPATCHER.init(*instance);
		Logger::info(logTag, "Instance created");
		return instance;
	}

	static vk::UniqueDebugUtilsMessengerEXT createDebugMessenger(const vk::Instance& instance) {
		assert(instance && "instance not initialized");

		if (isDebug) {
			const auto createInfo = createDebugMessengerCreateInfo();
			auto debugMessenger = instance.createDebugUtilsMessengerEXTUnique(createInfo);
			Logger::info(logTag, "Debug messenger created");
			return debugMessenger;
		}
		return vk::UniqueDebugUtilsMessengerEXT();
	}

	class VulkanInstance::Impl {
	public:

		Impl() :
			loader(createLoader()),
			instance(createInstance()),
			debugMessenger(createDebugMessenger(*instance)) { }

	private:
		vk::DynamicLoader loader;
		vk::UniqueInstance instance;
		vk::UniqueDebugUtilsMessengerEXT debugMessenger;

		friend VulkanInstance;
	};

	VulkanInstance::VulkanInstance() : pimpl(make_unique_pimpl<VulkanInstance::Impl>()) {

	}
	const vk::Instance& VulkanInstance::getInstance() const {
		return *pimpl->instance;
	}

}

