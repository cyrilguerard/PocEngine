#include "vulkan-device.hpp"

#include <cassert>
#include <set>
#include <optional>

#include "../../core/logger.hpp"

using namespace poc;

namespace poc {

	static constexpr char logTag[]{ "POC::VulkanDevice" };

	struct QueueConfig
	{
		std::optional<uint32_t> graphicsQueueIndex;
		std::optional<uint32_t> presentationQueueIndex;

		bool isComplete() const {
			return graphicsQueueIndex.has_value() && presentationQueueIndex.has_value();
		}

		bool useSameQueue() const {
			return isComplete() && (graphicsQueueIndex == presentationQueueIndex);
		}

	};

	static QueueConfig getQueueConfig(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface) {
		assert(physicalDevice && "physicalDevice not initialized");
		assert(surface && "surface not initialized");

		QueueConfig config{};
		const std::vector<vk::QueueFamilyProperties> queueFamilies = physicalDevice.getQueueFamilyProperties();

		for (uint32_t i = 0; i < queueFamilies.size(); ++i) {

			const auto queueFamily = queueFamilies[i];
			if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
				config.graphicsQueueIndex = i;
			}

			if (physicalDevice.getSurfaceSupportKHR(i, surface)) {
				config.presentationQueueIndex = i;
			}

			// best option is to use the same queue for graphic & presentation for better performance
			if (config.isComplete() && config.useSameQueue()) {
				break;
			}

		}

		assert(config.isComplete() && "physical is not compatible");
		return config;

	}

	static vk::UniqueDevice createDevice(const QueueConfig& config, const vk::PhysicalDevice& physicalDevice) {
		assert(physicalDevice && "physicalDevice not initialized");

		const std::set<uint32_t> queueIndexes{ *config.graphicsQueueIndex, *config.presentationQueueIndex };

		const float queuePriority = 1.0f;
		std::vector<vk::DeviceQueueCreateInfo> queueInfos(queueIndexes.size());
		std::for_each(queueIndexes.cbegin(), queueIndexes.cend(),
			[i = 0, &queueInfos, &queuePriority](uint32_t queueIndex) mutable {
			queueInfos[i]
				.setQueueFamilyIndex(queueIndex)
				.setPQueuePriorities(&queuePriority)
				.setQueueCount(1);
			i++;
		});

		const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		auto createInfo = vk::DeviceCreateInfo()
			.setQueueCreateInfoCount(static_cast<uint32_t>(queueInfos.size()))
			.setPQueueCreateInfos(queueInfos.data())
			.setEnabledExtensionCount(static_cast<uint32_t>(deviceExtensions.size()))
			.setPpEnabledExtensionNames(deviceExtensions.data());

		return physicalDevice.createDeviceUnique(createInfo);
	}

	static vk::Queue getQueue(const vk::Device& device, const uint32_t queueIndex) {
		assert(device && "device not initialized");
		return device.getQueue(queueIndex, 0);
	}

	class VulkanDevice::Impl {
	public:

		Impl(const VulkanPhysicalDevice& physicalDevice, const VulkanSurface& surface) :
			queueConfig(getQueueConfig(physicalDevice.getPhysicalDevice(), surface.getSurface())),
			device(createDevice(queueConfig, physicalDevice.getPhysicalDevice())),
			graphicQueue(getQueue(*device, *queueConfig.graphicsQueueIndex)),
			presentationQueue(getQueue(*device, *queueConfig.presentationQueueIndex)) {

			Logger::info(logTag, "Device created");
		}

		std::vector<vk::UniqueFence> createFences(const uint32_t nbFences) const {
			std::vector<vk::UniqueFence> fences;
			fences.reserve(nbFences);

			const auto createInfo = vk::FenceCreateInfo().setFlags(vk::FenceCreateFlagBits::eSignaled);
			for (uint32_t i = 0; i < nbFences; ++i) {
				fences.push_back(device->createFenceUnique(createInfo));
			}

			return fences;
		}

		std::vector<vk::UniqueSemaphore> createSemaphores(const uint32_t nbSemaphores) const {
			std::vector<vk::UniqueSemaphore> semaphores;
			semaphores.reserve(nbSemaphores);

			const auto createInfo = vk::SemaphoreCreateInfo();
			for (uint32_t i = 0; i < nbSemaphores; ++i) {
				semaphores.push_back(device->createSemaphoreUnique(createInfo));
			}

			return semaphores;
		}

	private:
		QueueConfig queueConfig;
		vk::UniqueDevice device;
		vk::Queue graphicQueue;
		vk::Queue presentationQueue;

		friend VulkanDevice;
	};

	VulkanDevice::VulkanDevice(const VulkanPhysicalDevice& physicalDevice, const VulkanSurface& surface) :
		pimpl(make_unique_pimpl<VulkanDevice::Impl>(physicalDevice, surface)) { }

	const vk::Device VulkanDevice::getDevice() const {
		return *pimpl->device;
	}

	uint32_t VulkanDevice::getGraphicsQueueIndex() const {
		return pimpl->queueConfig.graphicsQueueIndex.value();
	}

	const vk::Queue& VulkanDevice::getGraphicsQueue() const {
		return pimpl->graphicQueue;
	}

	uint32_t VulkanDevice::getPresentationQueueIndex() const {
		return pimpl->queueConfig.presentationQueueIndex.value();
	}

	const vk::Queue& VulkanDevice::getPresentationQueue() const {
		return pimpl->presentationQueue;
	}

	bool VulkanDevice::hasDistinctPresentationQueue() const {
		return !pimpl->queueConfig.useSameQueue();
	}

	std::vector<vk::UniqueFence> VulkanDevice::createFences(const uint32_t nbFences) const {
		return pimpl->createFences(nbFences);
	}

	std::vector<vk::UniqueSemaphore> VulkanDevice::createSemaphores(const uint32_t nbSemaphores) const {
		return pimpl->createSemaphores(nbSemaphores);
	}

}

