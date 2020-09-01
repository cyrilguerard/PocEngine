#include "vulkan-api.hpp"

#include "../../core/logger.hpp"
#include "vulkan-device.hpp"
#include "vulkan-instance.hpp"
#include "vulkan-physical-device.hpp"
#include "vulkan-surface.hpp"

using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanApi" };

	class VulkanApi::Impl {
	public:

		Impl(const Window& window) :
			instance(),
			surface(instance, window),
			physicalDevice(instance, surface),
			device(physicalDevice, surface) {

			Logger::info(logTag, "Vulkan API fully initialized");
		}

	private:
		VulkanInstance instance;
		VulkanSurface surface;
		VulkanPhysicalDevice physicalDevice;
		VulkanDevice device;
	};

	VulkanApi::VulkanApi(const Window& window) :
		pimpl(make_unique_pimpl<VulkanApi::Impl>(window)) {};

	void VulkanApi::render() {};

}

