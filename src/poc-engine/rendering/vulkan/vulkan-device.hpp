#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"
#include "vulkan-physical-device.hpp"

namespace poc {

	class VulkanDevice {
	public:

		VulkanDevice(const VulkanPhysicalDevice& physicalDevice, const VulkanSurface& surface);

		const vk::Device getDevice() const;

		const bool hasDistinctPresentationQueue() const;
		const uint32_t getGraphicsQueueIndex() const;
		const uint32_t getPresentationQueueIndex() const;

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}