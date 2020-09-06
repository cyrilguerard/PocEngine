#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"
#include "vulkan-physical-device.hpp"

namespace poc {

	class VulkanDevice {
	public:

		VulkanDevice(const VulkanPhysicalDevice& physicalDevice, const VulkanSurface& surface);

		const vk::Device getDevice() const;

		const uint32_t getGraphicsQueueIndex() const;
		const vk::Queue& getGraphicsQueue() const;

		const uint32_t getPresentationQueueIndex() const;
		const vk::Queue& getPresentationQueue() const;

		const bool hasDistinctPresentationQueue() const;

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}