#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"
#include "vulkan-physical-device.hpp"

namespace poc {

	class VulkanDevice {
	public:

		explicit VulkanDevice(const VulkanPhysicalDevice& physicalDevice, const VulkanSurface& surface);

		const vk::Device getDevice() const;

		uint32_t getGraphicsQueueIndex() const;
		const vk::Queue& getGraphicsQueue() const;

		uint32_t getPresentationQueueIndex() const;
		const vk::Queue& getPresentationQueue() const;

		bool hasDistinctPresentationQueue() const;

		std::vector<vk::UniqueFence> createFences(const uint32_t nbFences) const;
		std::vector<vk::UniqueSemaphore> createSemaphores(const uint32_t nbSemaphores) const;

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}