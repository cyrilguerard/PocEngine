#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../core/scene.hpp"
#include "../../plateform/platform.hpp"
#include "../../plateform/window.hpp"
#include "vulkan-command-pool.hpp"
#include "vulkan-device.hpp"
#include "vulkan-physical-device.hpp"
#include "vulkan-scene.hpp"
#include "vulkan-surface.hpp"

namespace poc {

	class VulkanRender {
	public:

		explicit VulkanRender(
			const Window& window,
			const VulkanPhysicalDevice& physicalDevice,
			const VulkanDevice& device,
			const VulkanSurface& surface,
			const VulkanCommandPool& commandPool,
			const vk::SwapchainKHR& oldSwapchain = nullptr);

		bool render(const VulkanDevice& device, const VulkanScene& scene) const;

		VulkanRender recreate(
			const Window& window,
			const VulkanPhysicalDevice& physicalDevice,
			const VulkanDevice& device,
			const VulkanSurface& surface,
			const VulkanCommandPool& commandPool);

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}