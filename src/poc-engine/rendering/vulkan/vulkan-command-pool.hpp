#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"
#include "vulkan-device.hpp"

namespace poc {

	class VulkanCommandPool {
	public:

		explicit VulkanCommandPool(const VulkanDevice& device);

		std::vector<vk::UniqueCommandBuffer> createCommandBuffers(const VulkanDevice& device, const uint32_t count) const;

		vk::UniqueCommandBuffer beginCommandBuffer(const VulkanDevice& device) const;
		void endCommandBuffer(const VulkanDevice& device, const vk::CommandBuffer& commandBuffer) const;

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};

}