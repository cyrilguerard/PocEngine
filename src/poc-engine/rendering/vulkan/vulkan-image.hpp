#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"

#include "vulkan-command-pool.hpp"
#include "vulkan-device.hpp"

namespace poc {

	class VulkanImage {
	public:

		explicit VulkanImage(
			const VulkanCommandPool& commandPool,
			const VulkanPhysicalDevice& physicalDevice,
			const VulkanDevice& device,
			const vk::Format& format,
			const uint32_t width,
			const uint32_t height,
			const vk::ImageTiling& tiling,
			const vk::ImageUsageFlags& usage,
			const vk::MemoryPropertyFlags& memoryProperties,
			const vk::ImageLayout& imageLayout);

		const vk::Image getImage() const;
		const vk::Format getFormat() const;

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}