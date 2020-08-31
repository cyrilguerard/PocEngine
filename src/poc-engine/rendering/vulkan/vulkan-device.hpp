#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"
#include "vulkan-physical-device.hpp"

namespace poc {

	class VulkanDevice {
	public:

		VulkanDevice(const VulkanPhysicalDevice& physicalDevice, const VulkanSurface& surface);

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}