#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"
#include "vulkan-device.hpp"

namespace poc {

	class VulkanCommandPool {
	public:

		VulkanCommandPool(const VulkanDevice& device);

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}