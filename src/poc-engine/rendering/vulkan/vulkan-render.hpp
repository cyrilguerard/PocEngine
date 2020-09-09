#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"
#include "../../plateform/window.hpp"
#include "vulkan-command-pool.hpp"
#include "vulkan-device.hpp"
#include "vulkan-physical-device.hpp"
#include "vulkan-surface.hpp"

namespace poc {

	class VulkanRender {
	public:

		explicit VulkanRender(
			const Window& window,
			const VulkanPhysicalDevice& physicalDevice,
			const VulkanDevice& device,
			const VulkanSurface& surface,
			const VulkanCommandPool& commandPool);

		void render(const VulkanDevice& device);

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}