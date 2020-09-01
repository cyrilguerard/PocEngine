#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"
#include "../../plateform/window.hpp"
#include "vulkan-device.hpp"
#include "vulkan-physical-device.hpp"
#include "vulkan-surface.hpp"

namespace poc {

	class VulkanSwapchain {
	public:

		VulkanSwapchain(
			const VulkanDevice& device,
			const VulkanPhysicalDevice& physicalDevice,
			const VulkanSurface& surface,
			const Window& window);

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}