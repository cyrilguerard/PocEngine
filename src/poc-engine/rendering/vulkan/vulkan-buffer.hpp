#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"

#include "vulkan-command-pool.hpp"
#include "vulkan-device.hpp"
#include "vulkan-physical-device.hpp"

namespace poc {

	class VulkanBuffer {
	public:

		explicit VulkanBuffer(
			const VulkanPhysicalDevice& physicalDevice,
			const VulkanDevice& device,
			const vk::DeviceSize& size,
			const vk::BufferUsageFlags& usage,
			const vk::MemoryPropertyFlags& memoryProperty,
			const void* data);

		const vk::Buffer& getBuffer() const;

		static VulkanBuffer createDeviceLocalBuffer(
			const VulkanPhysicalDevice& physicalDevice,
			const VulkanDevice& device,
			const VulkanCommandPool& commandPool,
			const vk::DeviceSize& size,
			const vk::BufferUsageFlags& usage,
			const void* data);

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}