#include "vulkan-api.hpp"

#include "../../core/logger.hpp"
#include "vulkan-device.hpp"
#include "vulkan-instance.hpp"
#include "vulkan-physical-device.hpp"
#include "vulkan-surface.hpp"
#include "vulkan-swapchain.hpp"

using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanApi" };

	class VulkanApi::Impl {
	public:

		Impl(const Window& window) :
			instance(),
			surface(instance, window),
			physicalDevice(instance, surface),
			device(physicalDevice, surface),
			swapchain(device, physicalDevice, surface, window) {

			Logger::info(logTag, "Vulkan API fully initialized");
		}

	private:
		VulkanInstance instance;
		VulkanSurface surface;
		VulkanPhysicalDevice physicalDevice;
		VulkanDevice device;
		VulkanSwapchain swapchain;
	};

	VulkanApi::VulkanApi(const Window& window) :
		pimpl(make_unique_pimpl<VulkanApi::Impl>(window)) {};

	void VulkanApi::render() {};

}

