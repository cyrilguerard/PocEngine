#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"
#include "../../plateform/window.hpp"
#include "vulkan-device.hpp"
#include "vulkan-image-view.hpp"
#include "vulkan-physical-device.hpp"
#include "vulkan-surface.hpp"

namespace poc {

	class VulkanSwapchain {
	public:

		explicit VulkanSwapchain(
			const Window& window,
			const VulkanPhysicalDevice& physicalDevice,
			const VulkanDevice& device,
			const VulkanSurface& surface,
			const vk::SwapchainKHR& oldSwapchain);

		const vk::SwapchainKHR& getSwapchain() const;
		const vk::Format& getFormat() const;
		const vk::Extent2D& getExtent() const;

		const uint32_t getNumberOfImages() const;
		const std::vector<VulkanImageView>& getImageViews() const;

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}