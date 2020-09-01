#include "vulkan-physical-device.hpp"

#include <algorithm>
#include <set>
#include <vector>

#include "../../core/logger.hpp"

using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanPhysicalDevice" };

	static bool isRequiredQueueFamiliesProvidedBy(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface) {
		const std::vector<vk::QueueFamilyProperties> properties = physicalDevice.getQueueFamilyProperties();

		const bool graphicQueueFound = std::any_of(properties.cbegin(), properties.cend(), [](const auto qf) {
			return (qf.queueFlags & vk::QueueFlagBits::eGraphics);
			});

		const bool presentationQueueFound = std::any_of(properties.cbegin(), properties.cend(), [i = 0, &physicalDevice, &surface](const auto) mutable {
			return physicalDevice.getSurfaceSupportKHR(i++, surface);
		});

		return graphicQueueFound && presentationQueueFound;
	}

	static bool isRequiredExtensionsSupportedBy(const vk::PhysicalDevice device) {
		const std::vector<std::string> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		const std::vector<vk::ExtensionProperties> extensions = device.enumerateDeviceExtensionProperties();
		return std::any_of(extensions.cbegin(), extensions.cend(),
			[](const auto ep) {
				return strcmp(VK_KHR_SWAPCHAIN_EXTENSION_NAME, ep.extensionName);
			});

	}

	bool isSurfaceCompatibleWith(const vk::PhysicalDevice device, const vk::SurfaceKHR& surface) {
		const auto formats = device.getSurfaceFormatsKHR(surface);
		const auto modes = device.getSurfacePresentModesKHR(surface);
		return !formats.empty() && !modes.empty();
	}

	static bool isDeviceSuitable(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface) {
		return isRequiredExtensionsSupportedBy(physicalDevice) &&
			isRequiredQueueFamiliesProvidedBy(physicalDevice, surface) &&
			isSurfaceCompatibleWith(physicalDevice, surface);
	}

	static vk::PhysicalDevice selectPhysicalDevice(const vk::Instance& instance, const vk::SurfaceKHR& surface) {
		assert(instance && "instance not initialized");
		assert(surface && "surface not initialized");

		std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices();
		if (devices.empty()) {
			Logger::error(logTag, "No GPU available");
			throw std::runtime_error("No GPU available");
		}

		// keep only suitable device
		(void)std::remove_if(devices.begin(), devices.end(), [&](const auto pd) { return !isDeviceSuitable(pd, surface); });
		if (devices.empty()) {
			Logger::error(logTag, "No compatible GPU found");
			throw std::runtime_error("No compatible GPU found");
		}

		// use discrete CPU in priority as more powerful than integrated CPU 
		const auto it = std::find_if(devices.cbegin(), devices.cend(),
			[](const auto pd) { return pd.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu; });
		if (it != devices.cend()) {
			return *it;
		}

		// take the first one
		return devices[0];

	}

	class VulkanPhysicalDevice::Impl {
	public:

		Impl(const VulkanInstance& instance, const VulkanSurface& surface) :
			physicalDevice(selectPhysicalDevice(instance.getInstance(), surface.getSurface())) {

			Logger::info(logTag, "GPU chosen: " + std::string(physicalDevice.getProperties().deviceName));
		}

	private:
		vk::PhysicalDevice physicalDevice;

		friend VulkanPhysicalDevice;
	};

	VulkanPhysicalDevice::VulkanPhysicalDevice(const VulkanInstance& instance, const VulkanSurface& surface) :
		pimpl(make_unique_pimpl<VulkanPhysicalDevice::Impl>(instance, surface)) {}

	const vk::PhysicalDevice VulkanPhysicalDevice::getPhysicalDevice() const {
		return pimpl->physicalDevice;
	}

}

