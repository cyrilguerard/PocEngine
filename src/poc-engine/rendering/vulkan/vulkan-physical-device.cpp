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

	static bool isSurfaceCompatibleWith(const vk::PhysicalDevice device, const vk::SurfaceKHR& surface) {
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

	static vk::Format selectDepthFormat(const vk::PhysicalDevice& physicalDevice) {

		assert(physicalDevice && "physicalDevice not initialized");

		// use at least 24 bit for depth, stencil is not used
		const std::array<vk::Format, 3> formats = {
			vk::Format::eD32Sfloat,
			vk::Format::eD32SfloatS8Uint,
			vk::Format::eD24UnormS8Uint
		};

		const auto it = std::find_if(formats.cbegin(), formats.cend(), [&physicalDevice](const auto format) {
			const vk::FormatProperties properties{ physicalDevice.getFormatProperties(format) };
			const vk::FormatFeatureFlagBits feature = vk::FormatFeatureFlagBits::eDepthStencilAttachment;
			return (properties.optimalTilingFeatures & feature) == feature;
			});

		if (it == formats.cend()) {
			Logger::error(logTag, "No suitable depth format");
			throw std::runtime_error("No suitable depth format");
		}

		return *it;
	}

	static vk::SampleCountFlagBits computeMaxSampleCount(const vk::PhysicalDevice& physicalDevice) {

		assert(physicalDevice && "physicalDevice not initialized");

		const vk::PhysicalDeviceLimits limits = physicalDevice.getProperties().limits;

		// check the max sample count supported by color & depth
		const vk::SampleCountFlags supportedSampleCount =
			limits.sampledImageColorSampleCounts &
			limits.sampledImageDepthSampleCounts;

		const std::array sampleCounts = {
			vk::SampleCountFlagBits::e64,
			vk::SampleCountFlagBits::e32,
			vk::SampleCountFlagBits::e16,
			vk::SampleCountFlagBits::e8,
			vk::SampleCountFlagBits::e4,
			vk::SampleCountFlagBits::e2
		};

		const auto it = std::find_if(sampleCounts.cbegin(), sampleCounts.cend(),
			[&sampleCounts, &supportedSampleCount](const auto sc) {
				return supportedSampleCount & sc;
			});

		return it != sampleCounts.cend() ? *it : vk::SampleCountFlagBits::e1;
	}

	class VulkanPhysicalDevice::Impl {
	public:

		const vk::PhysicalDevice physicalDevice;
		const vk::Format depthFormat;
		const vk::SampleCountFlagBits maxSampleCount;

		Impl(const VulkanInstance& instance, const VulkanSurface& surface) :
			physicalDevice(selectPhysicalDevice(instance.getInstance(), surface.getSurface())),
			depthFormat(selectDepthFormat(physicalDevice)),
			maxSampleCount(computeMaxSampleCount(physicalDevice)) {

			Logger::info(logTag, "GPU chosen: " + std::string(physicalDevice.getProperties().deviceName));
			Logger::info(logTag, "Depth format used: " + std::string(vk::to_string(depthFormat)));
			Logger::info(logTag, "Max sample count: " + std::string(vk::to_string(maxSampleCount)));
		}

		uint32_t findMemoryTypeIndex(uint32_t type, vk::MemoryPropertyFlags properties) {
			vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();
			for (uint32_t index = 0; index < memoryProperties.memoryTypeCount; ++index) {
				if ((type & (1 << index)) && (memoryProperties.memoryTypes[index].propertyFlags & properties) == properties) {
					return index;
				}
			}
			Logger::error(logTag, "Failed to find suitable memory type.");
			throw std::runtime_error("Failed to find suitable memory type.");
		}

	};

	VulkanPhysicalDevice::VulkanPhysicalDevice(const VulkanInstance& instance, const VulkanSurface& surface) :
		pimpl(make_unique_pimpl<VulkanPhysicalDevice::Impl>(instance, surface)) {}

	const vk::PhysicalDevice& VulkanPhysicalDevice::getPhysicalDevice() const {
		return pimpl->physicalDevice;
	}

	const uint32_t VulkanPhysicalDevice::findMemoryTypeIndex(uint32_t type, vk::MemoryPropertyFlags properties) const {
		return pimpl->findMemoryTypeIndex(type, properties);
	}

	const vk::Format& VulkanPhysicalDevice::getDepthFormat() const {
		return pimpl->depthFormat;
	}

	const vk::SampleCountFlagBits& VulkanPhysicalDevice::getMaxSampleCount() const {
		return pimpl->maxSampleCount;
	}

}

